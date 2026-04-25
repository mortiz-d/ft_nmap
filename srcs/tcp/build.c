
#include "../lib/nmap.h"

unsigned short checksum(char *b, int len) {
    unsigned short *buf = (unsigned short *)b;
    unsigned int sum = 0;

    //(Seguimos el estandard (RFC 1071) y sumamos de 2 bytes en 2 bytes
    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }

    if (len == 1)
        sum += *(unsigned char *)buf;  // último byte si es impar

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    return ~sum;
}

void compute_tcp_checksum(struct iphdr *ip, struct tcphdr *tcp)
{
    t_tcp_checksum tcp_checksum;
    char buf[4096];

    tcp_checksum.src = ip->saddr;
    tcp_checksum.dst = ip->daddr;
    tcp_checksum.zero = 0;
    tcp_checksum.protocol = IPPROTO_TCP;
    tcp_checksum.tcp_length = htons(sizeof(struct tcphdr));

    ft_memcpy(buf, &tcp_checksum, sizeof(tcp_checksum));
    ft_memcpy(buf + sizeof(tcp_checksum), tcp, sizeof(struct tcphdr));

    tcp->check = checksum(buf, sizeof(tcp_checksum) + sizeof(struct tcphdr));
}

void build_ip_header(t_params *params,struct iphdr *ip, struct sockaddr_in dst)
{
    ip->version = 4;    //ipv4
    ip->ihl = 5;        //longitude of header = 5 chars (20 bytes)         
    ip->tos = 0;        //Type of service 0 = SCAN
    ip->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr)); //Total longitude of the sum of the ip header + tcp header
    ip->id = 1 ;//ID 
    ip->frag_off = 0;   //No packet fragmentation
    ip->ttl = 64;       //Time to live
    ip->protocol = IPPROTO_TCP; //Used protocol
    ip->check = 0; // Checksum (Will be put later)
    ip->saddr = inet_addr(params->internal_ip); //Origing IP
    ip->daddr = dst.sin_addr.s_addr;            //Destiny IP
}

void build_tcp_header(t_params *params,struct tcphdr *tcp, int port, t_scan type)
{
    ft_memset(tcp, 0, sizeof(struct tcphdr));

    tcp->source = htons(params->launch_port);   //From what port originates
    tcp->dest = htons(port);                    //Destiny port
    tcp->seq = htonl(1);                   //Nº sequence
    tcp->ack_seq = 0;                           
    tcp->doff = 5;                              //longitude of TCP header = 5 chars (20 bytes)        

    if (type == SYN_SCAN) 
        tcp->syn = 1;       //This activates the SYN flag
    tcp->window = htons(5840); //Buffer in bytes BEFORE recieving ACK, see https://en.wikipedia.org/wiki/TCP_window_scale_option
    tcp->check = 0;  // Checksum (Will be put later)
}

void build_packet(char *packet, t_params *params, struct sockaddr_in addr, int port, t_scan type)
{
    struct iphdr *ip = (struct iphdr *)packet;
    struct tcphdr *tcp = (struct tcphdr *)(packet + sizeof(struct iphdr));

    ft_memset(packet, 0, 4096);

    build_ip_header(params, ip, addr);
    debug_ip_header(ip);
    build_tcp_header(params,tcp, port, type);
    debug_tcp_header(tcp);
    compute_tcp_checksum(ip, tcp); //(Tanto rollo para hacer el checksum :v )
}