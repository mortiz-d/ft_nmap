#include "../../lib/nmap.h"

void scan_port_TCP(t_params *params, struct sockaddr_in addr, int port, t_scan scan){

    char packet[4096];
    int sockfd;

    if (scan != UDP_SCAN)
        sockfd = socket_connection_tcp(addr); //TCP

    if (sockfd < 0)
    {
        printf("Error: No se pudo crear el socket del puerto %i\n",port);
        return;
    }

    ft_bzero(packet, sizeof(packet));
    build_packet_tcp(packet,params,addr,port,scan);
    send_packet_tcp(sockfd, packet, addr);

    close(sockfd);
    ft_bzero(packet, sizeof(packet));
    return;
}

static t_scan_task *dequeue(t_scan_task **head){
    t_scan_task *task = *head;
    if (!task)
        return NULL;
    *head = (*head)->next;
    return task;
}

void *send_scans_TCP(void *args){
    struct s_scan_tasks *task_args = (struct s_scan_tasks *)args;
    struct sockaddr_in addr;

    while (1) {
        pthread_mutex_lock(task_args->queue_lock);
        t_scan_task *task = dequeue(&task_args->head);
        pthread_mutex_unlock(task_args->queue_lock);

        if (!task) return NULL;

        ft_memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;

        if (inet_pton(AF_INET, task->ip, &addr.sin_addr) <= 0)
            printf("Invalid IP -> %s\n", task->ip);
        else
        {
            scan_port_TCP(task_args->params,addr, task->port, (t_scan)task->scan);

            // delay for udp scan
            if ((t_scan)task->scan == UDP_SCAN)
                usleep(task_args->params->udp_delay_us);
        }

        // contamos solo tareas reales (tras procesarlas), asi 'expected' es exacto
        pthread_mutex_lock(task_args->queue_lock);
        task_args->params->n_packet_sended++;
        pthread_mutex_unlock(task_args->queue_lock);

        free(task->ip);
        free(task);
    }
    return NULL;
}

static void free_task_queue(t_scan_task *head)
{
    t_scan_task *tmp;

    while (head)
    {
        tmp = head->next;
        free(head->ip);
        free(head);
        head = tmp;
    }
}


// Does a run sending all requests and capturing/processing
static void run_scan_pass(t_params *args, t_scan_task *head, int expected)
{
    pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;
    struct bpf_program fp;
    pcap_if_t *dev_lst = NULL;
    pcap_t *handle;
    pthread_t *sender_threads;

    handle = capture_setup(args, &fp, &dev_lst);
    if (!handle){
        printf("Error: no se pudo iniciar la captura\n");
        free_task_queue(head);
        return;
    }

    struct s_scan_tasks task_args = {&queue_lock, args, head};
    sender_threads = malloc(sizeof(pthread_t) * args->threads);
    for (int i = 0; i < args->threads; ++i)
        pthread_create(&sender_threads[i], NULL, send_scans_TCP, &task_args);

    capture_listen(args, handle, dev_lst, &fp, expected);

    for (int i = 0; i < args->threads; ++i)
        pthread_join(sender_threads[i], NULL);
    free(sender_threads);
}

void main_scan_logic_tcp(t_params* args){
    t_list      *ips = *args->ip_list;
    t_scan_task *head, *tail, *ptr;
    t_scan *scan;
    char        *local_dst;
    int         task_count;

    local_dst = dns_lookup((char *)ips->content);
    if (local_dst){
        get_local_ip(local_dst, args->internal_ip);
        free(local_dst);
    }

    for (t_list *scans = *args->scan; scans; scans = scans->next)
    {
        scan = scans->content;
        if (*scan ==  UDP_SCAN)
            continue;
        for (t_list *ports = *args->ports; ports; ports = ports->next)
        {
            for (t_list *ips = *args->ip_list; ips; ips = ips->next)
            {
                t_scan *scan = scans->content;

                ptr = malloc(sizeof(t_scan_task));

                ptr->t_id = ++task_count;
                ptr->ip = ft_strdup((char *)ips->content);
                ptr->port = ((t_port *)ports->content)->port_nbr;
                ptr->scan = *scan;
                ptr->next = NULL;

                if (tail)
                    tail->next = ptr;
                else
                    head = ptr;

                tail = ptr;
            }
        }
    }

    if (DEBUG)
        ft_printf("Executing %i tasks in %i threads\n", task_count, args->threads);
    run_scan_pass(args, head, task_count);
}