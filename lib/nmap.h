#ifndef NMAP_H
# define NMAP_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#define NMAP_USSAGE_ERROR "./ft_nmap --help <args>\n"

#define DEBUG 0
#define NI_MAXHOST 1025

#define MIN_PORT_RANGE 0
#define MAX_PORT_RANGE 65535
#define SOURCE_PORT 52341
#define TIMEOUT_MS 3000


#include "../libft/lib/libft.h"
#include "../libft/lib/get_next_line.h"
#include "../libft/lib/flag.h"
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#include <netinet/ip.h> 
#include <netinet/ip_icmp.h>
#include <sys/time.h>
#include <netdb.h>
#include <pcap.h> 
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <pthread.h>


typedef struct s_tcp_checksum {
    uint32_t src;
    uint32_t dst;
    uint8_t zero;
    uint8_t protocol;
    uint16_t tcp_length;
} t_tcp_checksum;

typedef enum e_port_state
{
    PORT_UNKNOWN,
    PORT_OPEN,
    PORT_CLOSED,
    PORT_FILTERED,
    PORT_UNFILTERED,
    PORT_OPENFILTERED,
    PORT_UNCALLED
} t_port_state;

typedef enum e_scan
{
    SYN_SCAN, //SYN-ACK signal = Port open | RST signal = port closed | No anwser = filtered
    NUL_SCAN, //No anwser = Port closed | RST signal  = port open
    FIN_SCAN, //No anwser = Port open/filtered | RST signal = port closed
    XMAS_SCAN,//No anwser = Port open/filtered | RST signal = port closed
    ACK_SCAN, //No anwser = filtered | RST signal = unfiltered
    UDP_SCAN,
    SCAN_UNKNOWN
} t_scan;

typedef struct s_port {
    int    port_nbr;
} t_port;

typedef struct s_params
{
    char			internal_ip[INET_ADDRSTRLEN];   //IP
    int             threads;        //Nbr of threads to search in each port
    t_list  **scan;             //Tipo de scan
    t_list  **ip_list;      //list with IPs from a file
    t_list  **ports;            //list ports specified
    t_list  **results;
    int     n_ports;
    bool    help;
    // int     launch_port;
    t_scan  active_scan;
    char	*active_ip;   //IP currently scanning
    int     n_packet_sended;
    int     n_packet_recieved;


}	t_params;


// typedef struct s_result_scan_list
// {
    
// }	t_result_scan_list;

typedef struct s_result_scan
{
    int    port_nbr;
    t_port_state syn;
    t_port_state nul;
    t_port_state fin;
    t_port_state xmas;
    t_port_state ack;
    t_port_state udp;
}	t_result_scan;


//Probablemente innecesario si consigo filtrar que es que
typedef struct s_packet{
    int port;
    char *ip;
    int scan;
    struct tcphdr header;
} t_packet;

//FLAGS FUNCTIONS
t_list *flags_config (void);


//PARAMS
t_params *params_default_config (void);
t_params *get_params (int argc, char **argv);
void free_params(t_params *params);


//DEBUG
void debug_params(t_params *params);
void debug_ip_header(struct iphdr *ip);
void debug_tcp_header(struct tcphdr *tcp);

int nmap(t_params *params,char * str);

char *dns_lookup(char *host);
int get_local_ip(char *dest_ip, char *out_ip);
void generate_result_table(t_params *params);
void reset_all_results(t_list **results, t_list *scans);

unsigned short checksum(char *b, int len);
int scan_tcp(t_params *params, t_scan type, struct sockaddr_in addr, int port);
int socket_connection(t_params *params);

//TCP (BUILD -> SEND -> RECIEVE -> PROCESS)
void build_packet(char *packet, t_params *params, struct sockaddr_in addr, int port, t_scan type);
int send_packet(int sockfd, char *packet, struct sockaddr_in addr);
ssize_t recv_packet(int sockfd, char *buffer);

void main_scan_logic(t_params* args);

//CAPTURE
void capture_packets(t_params *params);

#endif