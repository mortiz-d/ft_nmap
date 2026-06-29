#include "../lib/nmap.h"
#include <stdio.h>

// uint16_t calculate_checksum(void *data, int len) {
//     unsigned short *buf = data;
//     unsigned int sum = 0;
//     unsigned short result;

//     for (sum = 0; len > 1; len -= 2)
//         sum += *buf++;
//     if (len == 1)
//         sum += *(unsigned char *)buf;
//     sum = (sum >> 16) + (sum & 0xFFFF);
//     sum += (sum >> 16);
//     result = ~sum;
//     return result;
// }


// void fill_packet_tcp(t_packet *pck, int port, char *ip, int scan){
//     ft_bzero(pck, sizeof(*pck));
//     pck->header.th_dport = htons(port);
//     // pck->header.th_ulen  = htons(sizeof(*pck));
//     pck->header.th_flags = TH_SYN;
//     // pck->header.syn = 1; //TODO falta set los scans correctamente
//     // pck->header.ack = 0;
//     pck->header.th_sum = calculate_checksum(pck, sizeof(*pck));

//     pck->ip = ip;
//     pck->port = port;
//     pck->scan = scan;
// }

// void scan_port(char *ip, int port, t_scan *scan){
//     t_packet packet;
//     struct sockaddr_in dest;

//     dest.sin_family = AF_INET;
//     dest.sin_port = htons(port);
//     if (!inet_pton(AF_INET, ip, &(dest.sin_addr))){
//         printf("Wrong address\n");
//         return;
//     }
//     ft_memset(dest.sin_zero, 0, sizeof(dest.sin_zero));

//     int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
//     if (sock < 0){
//         perror("ERR creating socket");
//         return;
//     }
//     fill_packet_tcp(&packet, port, ip, *scan);
//     int sndto = sendto(sock, &packet, sizeof(packet), 0, (struct sockaddr *)&dest, (socklen_t)sizeof(dest));
//     if (sndto < 1){
//         perror("ERR sending");
//     }else{
//         printf("sent %i bytes\n", sndto);
//     }
}

// t_scan_task *dequeue(t_scan_task **head){
//     t_scan_task *task = *head;
//     if (!task)
//         return NULL;
//     *head = (*head)->next;
//     return task;
// }

// void *send_scans(void *args){
//     struct s_scan_tasks *task_args = (struct s_scan_tasks *)args;
    
//     while (1) {
//         pthread_mutex_lock(task_args->queue_lock);
//         t_scan_task *task = dequeue(&task_args->head);
//         pthread_mutex_unlock(task_args->queue_lock);
        
//         if (!task) return NULL;

//         //PRUEBAS----
//         printf("%i %s p=%i s=%i\n", task->t_id, task->ip, task->port, task->scan);
//         sleep(1);
//         //----
        
//         // scan_port(task->ip, task->port, (t_scan *)&task->scan);
        
//         free(task->ip);
//         free(task);
//     }
//     return NULL;
// }