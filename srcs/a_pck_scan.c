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
    // printf("Socket creado para puerto %i\n",port);
    // (char *packet, t_params *params, struct sockaddr_in addr, int port, t_scan type)
    build_packet(packet,params,addr,port,scan);
    // build_packet(packet, params, addr, port, type);
    send_packet(sockfd, packet, addr);
    // recv_packet(sockfd,buffer);
  
    close(sockfd);
    return;
}

// void *send_scans(void *args){
//     t_params *params = (t_params *)args;
//     struct sockaddr_in addr;
//     // t_list *ips = *params->ip_list;
//     // t_list *scans = NULL;
//     // t_scan *scan = NULL;
//     t_list *ports = NULL;
//     t_port *port = NULL;
//     // printf("BOOM! %s\n",params->active_ip);
//     char *ip = params->active_ip;

//     ft_memset(&addr, 0, sizeof(addr));

//     addr.sin_family = AF_INET;

//     if (inet_pton(AF_INET, ip, &addr.sin_addr) <= 0)
//     {
//         printf("Invalid IP -> %s\n", ip);
//         return NULL;
//     }
//     get_local_ip(dns_lookup(ip), params->internal_ip);
    
        
//     ports = *(params->ports);
//     while (ports){
//         port = (t_port *)ports->content;
//         // printf("sending to ip = %s port = %i\n",ip,port->port_nbr);
//         scan_port(params,addr, port->port_nbr, params->active_scan);
//         ports = ports->next;
//     }
//     //mandar todos los paquetes sin esperar
//     return NULL;
// }

void *send_scans(void *args)
{
    t_params *params = (t_params *)args;
    struct sockaddr_in addr;
    t_list *ports;
    t_port *port;

    ft_memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;

    if (inet_pton(AF_INET,
                  params->active_ip,
                  &addr.sin_addr) <= 0)
    {
        printf("Invalid IP -> %s\n", params->active_ip);
        return NULL;
    }

    ports = *(params->ports);

    while (ports)
    {
        port = (t_port *)ports->content;

        scan_port(params,
                  addr,
                  port->port_nbr,
                  params->active_scan);

        ports = ports->next;
    }

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

// static void clean_result_table(t_list *lst)
// {
//     t_result_scan *s;
//     while (lst)
//     {
//         s = (t_result_scan *)lst->content;
//         s->syn = PORT_UNKNOWN; 
//         s->nul = PORT_UNKNOWN;
//         s->fin = PORT_UNKNOWN;
//         s->xmas= PORT_UNKNOWN; 
//         s->ack = PORT_UNKNOWN;
//         s->udp = PORT_UNKNOWN;
//         lst = lst->next;
//     }
// }

void main_scan_logic(t_params* args){
    pthread_t sender_thread;
    pthread_t receiver_thread;

    t_list *scans = NULL;
    scans = *args->scan;

    while (scans)
    {
        ((t_params *)args)->active_scan = *(t_scan *)scans->content;
        printf("Swiched to another scan %i \n", *(t_scan *)scans->content);
        

        pthread_create(&receiver_thread, NULL, receive_scans, args);
        sleep(1);
        pthread_create(&sender_thread, NULL, send_scans, args);

        pthread_join(sender_thread, NULL);
        pthread_join(receiver_thread, NULL);
        
        scans = scans->next;
    }
    print_result_table(*args->results);
    reset_all_results(args->results, *args->scan);
    // clean_result_table(*args->results);
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