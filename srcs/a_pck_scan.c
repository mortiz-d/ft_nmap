#include "../lib/nmap.h"

uint16_t calculate_checksum(void *data, int len) {
    unsigned short *buf = data;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char *)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

//Desechar probablemente
void fill_packet_tcp(t_packet *pck, int port, char *ip, int scan){
    ft_bzero(pck, sizeof(*pck));
    pck->header.th_dport = htons(port);
    // pck->header.th_ulen  = htons(sizeof(*pck));
    pck->header.th_flags = TH_SYN;
    pck->header.th_off = 5;
    // pck->header.syn = 1;
    // pck->header.ack = 0;
    pck->header.th_sum = calculate_checksum(pck, sizeof(*pck));

    pck->ip = ip;
    pck->port = port;
    pck->scan = scan;
}

void scan_port(t_params *params, struct sockaddr_in addr, int port, t_scan scan){

    char packet[4096];
    int sockfd;

    if (scan != UDP_SCAN)
        sockfd = socket_connection_tcp(addr); //TCP
    else
        sockfd = socket_connection_udp(addr); //UDP


    if (sockfd < 0)
    {
        printf("Error: No se pudo crear el socket del puerto %i\n",port);
        return;
    }

    ft_bzero(packet, sizeof(packet));
    if (params->active_scan != UDP_SCAN)
    {
        build_packet_tcp(packet,params,addr,port,scan);
        send_packet_tcp(sockfd, packet, addr);
    }
    else
    {
        send_probe_udp(sockfd,addr,params,port);
    }

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

void *send_scans(void *args){
    struct s_scan_tasks *task_args = (struct s_scan_tasks *)args;
    struct sockaddr_in addr;
    
    while (1) {
        pthread_mutex_lock(task_args->queue_lock);
        t_scan_task *task = dequeue(&task_args->head);
        task_args->params->n_packet_sended++;
        pthread_mutex_unlock(task_args->queue_lock);
        
        if (!task) return NULL;

        //PRUEBAS----
        // printf("sended %i %s p=%i s=%i\n", task->t_id, task->ip, task->port, task->scan);
        // sleep(1);
        //----


        ft_memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
    
        if (inet_pton(AF_INET, task->ip, &addr.sin_addr) <= 0)
        {
            printf("Invalid IP -> %s\n", task->ip);
            return NULL;
        }


        scan_port(task_args->params,addr, task->port, (t_scan)task->scan);
        
        free(task->ip);
        free(task);
    }
    return NULL;
}

void main_scan_logic(t_params* args){
    t_list *ips = *args->ip_list;
    char *ip = NULL;

    t_port *prt = NULL;
    int port = 0;
    t_scan *scan = NULL;

    t_scan_task *head = NULL;
    t_scan_task *tail = NULL;
    t_scan_task *ptr = NULL;

    pthread_mutex_t queue_lock = PTHREAD_MUTEX_INITIALIZER;

    get_local_ip(dns_lookup((char *)ips->content), args->internal_ip);
    
    int task_count = 0;
    for (t_list *scans = *args->scan; scans; scans = scans->next){
        scan = (t_scan *)scans->content;
        ips = *args->ip_list;


        while (ips){
            
            ip = (char *)ips->content;
            
            for (t_list *ports = *args->ports; ports; ports = ports->next){
                prt = (t_port *)ports->content;
                port = prt->port_nbr;
                ++task_count;
                ptr = malloc(sizeof(t_scan_task));
                ptr->t_id = task_count;
                ptr->port = port;
                ptr->ip = ft_strdup(ip);
                ptr->scan = *scan;
                ptr->next = NULL;
                if (tail)
                    tail->next = ptr;
                else
                    head = ptr;
                tail = ptr;
                // printf("task %i created ip = %s scan = %i port = %i\n",task_count, ip,*scan, port);
            }
            ips = ips->next;
        }
        args->active_scan = *scan;

        if(DEBUG)
            printf("Executing %i tasks in %i threads\n", task_count, args->threads);

        struct bpf_program fp;
        pcap_if_t *dev_lst = NULL;
        pcap_t *handle = capture_setup(args, &fp, &dev_lst);
        if (!handle){
            printf("Error: no se pudo iniciar la captura\n");
            return;
        }

        struct s_scan_tasks task_args = {&queue_lock, args, head};
        pthread_t *sender_threads = malloc(sizeof(pthread_t) * args->threads);
        for (int i = 0; i < args->threads; ++i){
            pthread_create(&sender_threads[i], NULL, send_scans, &task_args);
        }

        capture_listen(args, handle, dev_lst, &fp);

        for (int i = 0; i < args->threads; ++i){
            pthread_join(sender_threads[i], NULL);
        }
        free(sender_threads);
        head = NULL;
        tail = NULL;
    }
}