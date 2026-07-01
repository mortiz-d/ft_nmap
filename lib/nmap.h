#ifndef NMAP_H
# define NMAP_H

#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L //Esto es para mi ordenador que puede actuar funny el hijopu
#define NMAP_USSAGE_ERROR "./ft_nmap --help <args>\n"

#define DEBUG 1
#define NI_MAXHOST 1025

#define MIN_PORT_RANGE 0
#define MAX_PORT_RANGE 65535
#define UDP_DEFAULT_BASE_PORT 33434
#define SOURCE_PORT 52341
#define TIMEOUT_MS 3000
#define UDP_PROBE_DELAY_US 100000
#define UDP_MAX_RETRIES 10
#define UDP_RETRY_WAIT_US 1500000
#define UDP_MAX_DELAY_US 1000000
#define UDP_DROP_THRESHOLD_PCT 30
#define EXEC_NAME "ft_nmap"

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
#include <netinet/udp.h>


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
    UDP_SCAN, //No anwser = Port open/filtered | ICMP type 3 code 3 = closed | UDP response = open
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
    t_scan  active_scan;
    char	*active_ip;   //IP currently scanning
    int     n_packet_sended;
    int     n_packet_recieved;
    int     udp_delay_us;   //Delay between udp probes


}	t_params;

typedef struct s_result_port
{
    int    port_nbr;
    t_port_state syn;
    t_port_state nul;
    t_port_state fin;
    t_port_state xmas;
    t_port_state ack;
    t_port_state udp;
}	t_result_port;

typedef struct s_result_scan
{
    char * ip;
    t_list **port;
}	t_result_scan;


typedef struct s_scan_task {
    int t_id;
    int port;
    int scan;
    char *ip;
    struct s_scan_task *next;
} t_scan_task;

struct s_scan_tasks{
    pthread_mutex_t *queue_lock;
    t_params *params;
    struct s_scan_task *head;
};


//FLAGS FUNCTIONS
t_list *flags_config (void);


//PARAMS
t_params *params_default_config (void);
void free_params(t_params *params);
void free_result(void *result);


//DEBUG
void debug_params(t_params *params);
void debug_ip_header(struct iphdr *ip);
void debug_tcp_header(struct tcphdr *tcp);
void debug_udp_header(struct udphdr *udp);

char *dns_lookup(char *host);
int get_local_ip(char *dest_ip, char *out_ip);
void generate_result_table(t_params *params);
void print_result_table(t_params *params);


//TCP (BUILD -> SEND -> RECIEVE -> PROCESS)
int socket_connection_tcp(struct sockaddr_in addr);
void build_packet_tcp(char *packet, t_params *params, struct sockaddr_in addr, int port, t_scan type);
int send_packet_tcp(int sockfd, char *packet, struct sockaddr_in addr);
void packet_handler_tcp(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt);

//UDP ( SEND -> RECIEVE -> PROCESS)
int socket_connection_udp(struct sockaddr_in addr);
int send_probe_udp(int sockfd ,struct sockaddr_in addr,t_params *params, int port);
void packet_handler_udp(u_char *args, const struct pcap_pkthdr *hdr, const u_char *pkt);

void modify_result_table (t_params *params, char *ip, uint16_t port, t_port_state state);

//SCAN
void main_scan_logic(t_params* args);

//CAPTURE
pcap_t *capture_setup(t_params *params, struct bpf_program *fp, pcap_if_t **dev_lst);
void capture_listen(t_params *params, pcap_t *handle, pcap_if_t *dev_lst, struct bpf_program *fp, int expected);

#endif