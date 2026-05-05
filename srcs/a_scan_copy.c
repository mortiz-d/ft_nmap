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


void fill_packet_tcp(t_packet *pck, int port, char *ip, int scan){
    ft_bzero(pck, sizeof(*pck));
    pck->header.th_dport = htons(port);
    // pck->header.th_ulen  = htons(sizeof(*pck));
    pck->header.th_flags = TH_SYN;
    // pck->header.syn = 1;
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

void *send_scans(void *args){
    t_params *params = (t_params *)args;

    t_list *ips = *params->ip_list;
    char *ip = NULL;

    // t_list *ports = NULL;
    // int port = 0;

    t_list *scans = NULL;
    t_scan *scan = NULL;

    while (ips){
        ip = (char *)ips->content;
        // ports = *(params->ports);
        // for (; ports; ports = ports->next){
        //     port = (int)ports->content;
        // }
        scans = *params->scan;
        while (scans){
            scan = (t_scan *)scans->content;
            printf("sending to ip = %s scan = %i\n",ip,*scan);
            scan_port(ip, 80, scan);
            scans = scans->next;
        }
        ips = ips->next;
    }
    //mandar todos los paquetes sin esperar
    return NULL;
}

void *receive_scans(void *args){
    t_params *params = (t_params *)args;
    (void)params; //QUITAR, es para que se calle el unused variable
    capture_packets();
    //usar la libreria pcap para capturar paquetes
    return NULL;
}

void main_scan_logic(t_params* args){
    pthread_t sender_thread;
    pthread_t receiver_thread;

    //puede haber N sender threads
    printf("THE SENDER?\n");
    pthread_create(&sender_thread, NULL, send_scans, args);
    printf("THE RECEIVER?\n");
    pthread_create(&receiver_thread, NULL, receive_scans, args);

    pthread_join(sender_thread, NULL);
    pthread_join(receiver_thread, NULL);
}
/*
void test_tcp(){
    int sock_r = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock_r < 0){
        perror("ERR creating socket");
        return;
    }
    struct timeval timeout = { .tv_sec = 5, .tv_usec = 0 };
    setsockopt(sock_r, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    // t_packet packet;
    char r_buffer[1024];
    // struct sockaddr_in ping_from;
    // socklen_t addrlen = sizeof(ping_from);

    struct sockaddr_in ping_to;
    ping_to.sin_family = AF_INET;
    ping_to.sin_port = htons(80);
    ft_memset(ping_to.sin_zero, 0, sizeof(ping_to.sin_zero));
    if (!inet_pton(AF_INET, "142.250.185.46", &(ping_to.sin_addr))){
        ft_printf("Wrong address\n");
        return;
    }

    if (connect(sock_r, (struct sockaddr *)&ping_to, sizeof(ping_to)) < 0) {
        perror("ERR connecting");
        return;
    }

    const char *msg = "GET / HTTP/1.0\r\n\r\n";
    int snd = send(sock_r, msg, strlen(msg), 0);
    if (snd < 0){
        perror("ERR sending");
        return;
    }
    int rec_bytes = recv(sock_r, r_buffer, sizeof(r_buffer) - 1, 0);
    if (rec_bytes < 0){
        perror("ERR rec");
        return;
    }else {
        r_buffer[rec_bytes] = '\0';
        ft_printf("Received:\n%s\n", r_buffer);
    }
}
 */
