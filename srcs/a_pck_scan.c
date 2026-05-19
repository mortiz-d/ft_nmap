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
        sockfd = socket_connection_tcp(params); //TCP
    else
        sockfd = socket_connection_udp(params); //UDP


    if (!sockfd)
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
        pthread_mutex_unlock(task_args->queue_lock);
        
        if (!task) return NULL;

        //PRUEBAS----
        // printf("%i %s p=%i s=%i\n", task->t_id, task->ip, task->port, task->scan);
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

/* OLD
void *send_scans(void *args)
{
    t_params *params = (t_params *)args;
    struct sockaddr_in addr;
    t_list *ports;
    t_port *port;

    ft_memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;

    if (inet_pton(AF_INET, params->active_ip, &addr.sin_addr) <= 0)
    {
        printf("Invalid IP -> %s\n", params->active_ip);
        return NULL;
    }

    ports = *(params->ports);

    while (ports)
    {
        port = (t_port *)ports->content;
        scan_port(params,addr,port->port_nbr,params->active_scan);
        ports = ports->next;
    }

    return NULL;
} */

//Este hilo ira recogiendo todos los 
/* void *receive_scans(void *args){
    t_params *params = (t_params *)args;
    (void)params; //QUITAR, es para que se calle el unused variable | NOT ANYMORE (mortiz)
    capture_packets(params);
    //usar la libreria pcap para capturar paquetes
    return NULL;
} */

static const char *port_state_str(t_port_state state)
{
    switch (state)
    {
        case PORT_OPEN:
            return "OPEN";
            break;
        case PORT_CLOSED:
            return "CLOSED";
            break;
        case PORT_FILTERED:
            return "FILTERED";
            break;
        case PORT_UNFILTERED:
            return "UNFILTERED";
            break; 
        case PORT_UNCALLED:
            return "----------";
            break;
        case PORT_OPENFILTERED:
            return "OP|FILT";
            break;
        
        default:
            break;
    }
    return "UNKNOWN";
}

void print_result_table(t_list *lst)
{
    t_result_scan *s;

    printf("\n");
    printf("+--------+------------+------------+------------+------------+------------+------------+\n");
    printf("| PORT   | SYN        | NULL       | FIN        | XMAS       | ACK        | UDP        |\n");
    printf("+--------+------------+------------+------------+------------+------------+------------+\n");

    while (lst)
    {
        s = (t_result_scan *)lst->content;

        printf("| %-6d | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s |\n",s->port_nbr,port_state_str(s->syn),port_state_str(s->nul),port_state_str(s->fin),port_state_str(s->xmas),port_state_str(s->ack),port_state_str(s->udp));
        lst = lst->next;
    }

    printf("+--------+------------+------------+------------+------------+------------+------------+\n");
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
    
    int task_count = 0;
    while (ips){
        ip = (char *)ips->content;
        
        for (t_list *ports = *args->ports; ports; ports = ports->next){
            prt = (t_port *)ports->content;
            port = prt->port_nbr;
            
            for (t_list *scans = *args->scan; scans; scans = scans->next){
                ++task_count;
                scan = (t_scan *)scans->content;
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
        }
        ips = ips->next;
    }
    if(DEBUG)
        printf("Executing %i tasks in %i threads\n", task_count, args->threads);

    struct s_scan_tasks task_args = {&queue_lock, args, head};
    pthread_t *sender_threads = malloc(sizeof(pthread_t) * args->threads);
    for (int i = 0; i < args->threads; ++i){
        pthread_create(&sender_threads[i], NULL, send_scans, &task_args);
    }
    capture_packets(args);
    
    for (int i = 0; i < args->threads; ++i){
        pthread_join(sender_threads[i], NULL);
    }

    print_result_table(*args->results);
    reset_all_results(args->results, *args->scan);
    // clean_result_table(*args->results);
}

/* OLD
void main_scan_logic(t_params* args){
    pthread_t sender_thread;
    pthread_t receiver_thread;

    t_list *scans = NULL;
    scans = *args->scan;
    // print_result_table(*args->results);
    while (scans)
    {
        ((t_params *)args)->active_scan = *(t_scan *)scans->content;
        printf("Swiched to another scan %i \n", *(t_scan *)scans->content);
        

        pthread_create(&receiver_thread, NULL, receive_scans, args);
        sleep(2);
        pthread_create(&sender_thread, NULL, send_scans, args);

        pthread_join(sender_thread, NULL);
        pthread_join(receiver_thread, NULL);
        
        scans = scans->next;
    }
    print_result_table(*args->results);
    reset_all_results(args->results, *args->scan);
    // clean_result_table(*args->results);
}
 */