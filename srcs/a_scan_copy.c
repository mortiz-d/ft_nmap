#include "../lib/nmap.h"
#include <stdio.h>

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


void fill_packet_tcp(t_packet *pck, int port, char *ip, int scan){
    ft_bzero(pck, sizeof(*pck));
    pck->header.th_dport = htons(port);
    // pck->header.th_ulen  = htons(sizeof(*pck));
    pck->header.th_flags = TH_SYN;
    // pck->header.syn = 1; //TODO falta set los scans correctamente
    // pck->header.ack = 0;
    pck->header.th_sum = calculate_checksum(pck, sizeof(*pck));

    pck->ip = ip;
    pck->port = port;
    pck->scan = scan;
}

void scan_port(char *ip, int port, t_scan *scan){
    t_packet packet;
    struct sockaddr_in dest;

    dest.sin_family = AF_INET;
    dest.sin_port = htons(port);
    if (!inet_pton(AF_INET, ip, &(dest.sin_addr))){
        printf("Wrong address\n");
        return;
    }
    ft_memset(dest.sin_zero, 0, sizeof(dest.sin_zero));

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0){
        perror("ERR creating socket");
        return;
    }
    fill_packet_tcp(&packet, port, ip, *scan);
    int sndto = sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr *)&dest, (socklen_t)sizeof(dest));
    if (sndto < 1){
        perror("ERR sending");
    }else{
        printf("sent %i bytes\n", sndto);
    }
}

t_scan_task *dequeue(t_scan_task **head){
    t_scan_task *task = *head;
    if (!task)
        return NULL;
    *head = (*head)->next;
    return task;
}

void *send_scans(void *args){
    struct s_scan_tasks *task_args = (struct s_scan_tasks *)args;
    
    while (1) {
        pthread_mutex_lock(task_args->queue_lock);
        t_scan_task *task = dequeue(&task_args->head);
        pthread_mutex_unlock(task_args->queue_lock);
        
        if (!task) return NULL;

        //PRUEBAS----
        printf("%i %s p=%i s=%i\n", task->t_id, task->ip, task->port, task->scan);
        sleep(1);
        //----
        
        // scan_port(task->ip, task->port, (t_scan *)&task->scan);
        
        free(task->ip);
        free(task);
    }
    return NULL;
}

/* void *receive_scans(void *args){
    t_params *params = (t_params *)args;
    (void)params; //QUITAR, es para que se calle el unused variable
    capture_packets();
    return NULL;
} */

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
                printf("task %i created ip = %s scan = %i port = %i\n",task_count, ip,*scan, port);
            }
        }
        ips = ips->next;
    }
    printf("Executing %i tasks in %i threads\n", task_count, args->threads);

    struct s_scan_tasks task_args = {&queue_lock, head};
    pthread_t *sender_threads = malloc(sizeof(pthread_t) * args->threads);
    for (int i = 0; i < args->threads; ++i){
        pthread_create(&sender_threads[i], NULL, send_scans, &task_args);
    }
    capture_packets(args);
    
    for (int i = 0; i < args->threads; ++i){
        pthread_join(sender_threads[i], NULL);
    }
}
