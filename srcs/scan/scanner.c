#include "../../lib/nmap.h"

static void scan_port_tcp(t_params *params, struct sockaddr_in addr, int port, t_scan scan)
{
    char packet[4096];
    int  sockfd;

    sockfd = socket_connection_tcp(addr);
    if (sockfd < 0)
    {
        printf("Error: No se pudo crear el socket del puerto %i\n", port);
        return;
    }

    ft_bzero(packet, sizeof(packet));
    build_packet_tcp(packet, params, addr, port, scan);
    send_packet_tcp(sockfd, packet, addr);
    close(sockfd);
}

static void scan_port_udp(t_params *params, struct sockaddr_in addr, int port)
{
    int sockfd;

    sockfd = socket_connection_udp(addr);
    if (sockfd < 0)
    {
        printf("Error: No se pudo crear el socket del puerto %i\n", port);
        return;
    }

    send_probe_udp(sockfd, addr, params, port);
    close(sockfd);
}

void scan_port(t_params *params, struct sockaddr_in addr, int port, t_scan scan){
    if (scan == UDP_SCAN)
        scan_port_udp(params, addr, port);
    else
        scan_port_tcp(params, addr, port, scan);
}

static t_scan_task *dequeue(t_scan_task **head){
    t_scan_task *task = *head;
    if (!task)
        return NULL;
    *head = (*head)->next;
    return task;
}

void *send_scans(void *args){
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
            scan_port(task_args->params,addr, task->port, (t_scan)task->scan);

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

// Makes a queue with all open|filtered udp ports (default state)
static int build_udp_retry_queue(t_params *args, t_scan_task **head, t_scan_task **tail, int scan)
{
    t_list          *res = *args->results;
    t_result_scan   *rs;
    t_list          *p;
    t_result_port   *rp;
    t_scan_task     *t;
    int             count = 0;

    *head = NULL;
    *tail = NULL;
    while (res)
    {
        rs = res->content;
        p = *rs->port;
        while (p)
        {
            rp = p->content;
            if (rp->states[UDP_SCAN] == PORT_OPENFILTERED)
            {
                t = malloc(sizeof(t_scan_task));
                t->t_id = ++count;
                t->port = rp->port_nbr;
                t->ip = ft_strdup(rs->ip);
                t->scan = scan;
                t->next = NULL;
                if (*tail)
                    (*tail)->next = t;
                else
                    *head = t;
                *tail = t;
            }
            p = p->next;
        }
        res = res->next;
    }
    return count;
}

// Does a run sending all requests and capturing/processing (UDP might need a second or third run...)
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
        pthread_create(&sender_threads[i], NULL, send_scans, &task_args);

    capture_listen(args, handle, dev_lst, &fp, expected);

    for (int i = 0; i < args->threads; ++i)
        pthread_join(sender_threads[i], NULL);
    free(sender_threads);
}

static void udp_retransmit(t_params *args, int scan, int total_ports)
{
    int prev_remaining = total_ports + 1;
    t_scan_task *rhead, *rtail;
    int remaining, sent , dropped , new_delay;

    for (int retry = 0; retry < UDP_MAX_RETRIES; ++retry){
        sent = args->n_packet_sended;
        dropped = sent - args->n_packet_recieved;

        if (sent > 0 && (dropped * 100 / sent) > UDP_DROP_THRESHOLD_PCT && args->udp_delay_us < UDP_MAX_DELAY_US)
        {
            new_delay = args->udp_delay_us * 2;
            if (new_delay > UDP_MAX_DELAY_US)
                new_delay = UDP_MAX_DELAY_US;
            if (DEBUG)
                printf("Increasing send delay: %i -> %i us (%i/%i probes dropped)\n", args->udp_delay_us, new_delay, dropped, sent);
            args->udp_delay_us = new_delay;
        }

        remaining = build_udp_retry_queue(args, &rhead, &rtail, scan);
        if (remaining == 0)
            break;

        // when there is no new updates that means we have run out of closed ports
        if (remaining >= prev_remaining && args->udp_delay_us >= UDP_MAX_DELAY_US){
            free_task_queue(rhead);
            break;
        }
        prev_remaining = remaining;

        if (DEBUG)
            printf("UDP retry %i/%i: %i puertos pendientes (delay %i us)\n",
                   retry + 1, UDP_MAX_RETRIES, remaining, args->udp_delay_us);
        usleep(UDP_RETRY_WAIT_US);
        run_scan_pass(args, rhead, remaining);
    }
}

void main_scan_logic(t_params* args){
    t_list      *ips = *args->ip_list;
    t_scan_task *head = NULL, *tail = NULL, *ptr;
    char        *local_dst;
    int         task_count = 0;

    local_dst = dns_lookup((char *)ips->content);
    if (local_dst){
        get_local_ip(local_dst, args->internal_ip);
        free(local_dst);
    }

    for (t_list *scans = *args->scan; scans; scans = scans->next)
    {
        for (t_list *ports = *args->ports; ports; ports = ports->next)
        {
            for (t_list *ips = *args->ip_list; ips; ips = ips->next)
            {
                t_scan *scan = scans->content;

                ptr = malloc(sizeof(t_scan_task));

                ptr->t_id = ++task_count;
                ptr->ip = ft_strdup((char *)ips->content);
                printf("BUSCAMOS %s\n",ptr->ip);
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

    if (args->udp_active)
        udp_retransmit(args, UDP_SCAN, task_count);
}