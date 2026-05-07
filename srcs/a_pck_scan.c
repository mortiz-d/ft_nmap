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
    (void) params;
    (void) addr;
    (void) port;
    (void) packet;
    int sockfd;
    
    
    sockfd = socket_connection(params);
    if (!sockfd)
    {
        printf("Error: No se pudo crear el socket del puerto %i\n",port);
        return;
    }
    ft_bzero(packet, sizeof(packet));
    printf("Socket creado para puerto %i\n",port);
    // (char *packet, t_params *params, struct sockaddr_in addr, int port, t_scan type)
    build_packet(packet,params,addr,port,scan);
    // build_packet(packet, params, addr, port, type);
    printf("DEBUG dst = %s\n", inet_ntoa(addr.sin_addr));
    send_packet(sockfd, packet, addr);
    // recv_packet(sockfd,buffer);
  
    close(sockfd);
    return;
}

void *send_scans(void *args){
    t_params *params = (t_params *)args;
    struct sockaddr_in addr;
    t_list *ips = *params->ip_list;
    t_list *scans = NULL;
    t_scan *scan = NULL;
    char *ip = NULL;

    while (ips){
        ip = (char *)ips->content;
        ft_memset(&addr, 0, sizeof(addr));

        addr.sin_family = AF_INET;

        if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
        {
            printf("Invalid IP -> %s\n", ip);
            ips = ips->next;
            continue;
        }

        // printf("Valid IP -> %s\n", ip);

        // aux = ;
        get_local_ip(dns_lookup(ip), params->internal_ip);
        
        // ports = *(params->ports);
        // for (; ports; ports = ports->next){
        //     port = (int)ports->content;
        // }
        scans = *params->scan;
        while (scans){
            scan = (t_scan *)scans->content;
            printf("sending to ip = %s scan = %i\n",ip,*scan);
            scan_port(params,addr, 80, *scan);
            scans = scans->next;
        }
        ips = ips->next;
    }
    //mandar todos los paquetes sin esperar
    return NULL;
}

//Este hilo ira recogiendo todos los 
void *receive_scans(void *args){
    t_params *params = (t_params *)args;
    (void)params; //QUITAR, es para que se calle el unused variable | NOT ANYMORE (mortiz)
    capture_packets(params);
    //usar la libreria pcap para capturar paquetes
    return NULL;
}

void main_scan_logic(t_params* args){
    pthread_t sender_thread;
    pthread_t receiver_thread;

    //puede haber N sender threads
    pthread_create(&receiver_thread, NULL, receive_scans, args);
    sleep(1);
    pthread_create(&sender_thread, NULL, send_scans, args);


    printf("?\n");
    pthread_join(sender_thread, NULL);
    printf("1?\n");
    pthread_join(receiver_thread, NULL);
    printf("2?\n");
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