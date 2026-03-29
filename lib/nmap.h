#ifndef NMAP_H
# define NMAP_H

#define _POSIX_C_SOURCE 200809L //Esto es para mi ordenador que puede actuar funny el hijopu
#define _DEFAULT_SOURCE
#define PING_USSAGE_ERROR "ping :usage error:a direction must be specified\n"
#define PING_ROOT_ERROR "ping :error: needs root privileges\n"
#define PING_CHECKSUM_ERROR "ping :ICMP checksum error\n"
#define PING_UNKNOWN_HOST_ERROR "ping :couldnt convert to ip host\n"

#define PAYLOAD_SIZE_DEFAULT 56 //Tamaño del mensaje en ICMP que queremos mandar
#define ICMPHDR (sizeof(struct icmphdr))
#define TOTAL_SIZE (ICMPHDR+sizeof(struct iphdr))

#define DEFAULT_INTERVAL_PING 1
#define DEFAULT_TTL 64
#define DEFAULT_TIMEOUT 1000
#define MAXIMUM_PACKET_SIZE 65535
#define NI_MAXHOST 1025

#define ICMP_ECHO_CODE 0
#define DEBUG 0

#include "../libft/lib/libft.h"
#include "../libft/lib/get_next_line.h"
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
//Para la capa 4
#include <netinet/tcp.h>


typedef enum e_port_state
{
    PORT_UNKNOWN,
    PORT_OPEN,
    PORT_CLOSED,
    PORT_FILTERED
} t_port_state;

typedef enum e_scan
{
    SYN,
    NUL,
    FIN,
    XMAS,
    ACK,
    UDP,
    SCAN_UNKNOWN
} t_scan;

typedef struct s_port {
    bool   range;
    int    port_nbr;
    int    port_min;
    int    port_max;
} t_port;

typedef struct s_params
{
    // struct timeval	start_ping;
	// struct timeval	end_ping;
    char			*destination;   //IP
    int             threads;        //Nbr of threads to search in each port
    // char            ip_address[INET_ADDRSTRLEN];
	// int				ttl;
    // int 			timeout_ms;
    // int				interval;
    // int 			count;
    // int				payload_size;
    // active_flags    *flags;
    t_list  **scan;             //Tipo de scan
    t_list  **filename_ip;      //list with IPs from a file
    t_list  **ports;            //list ports specified
    bool    help;
    
}	t_params;

typedef struct s_ping
{
	int				id_process;
	int				sequence;
    struct timeval	start;
	struct timeval	end;
}	t_ping;

typedef struct s_stat
{
	double      min;
    double      max;
    double      sum;
    int         count;
    int         count_rec;
    double      avg;
    double      mdev;
    double      mdev_aux;
}	t_stat;




//TCP
// int send_tcp(int sockfd, struct sockaddr_in *addr, char *src_ip, int port);
// void recv_tcp(int sockfd, t_packet *aux);
// bool process_tcp(int rec_bytes, t_packet *aux, char *recv_buf);
// void build_syn_packet(char *packet, char *src_ip, char *dst_ip, int port);


void handle_sigint(int sig);
void ping_loop (int socketfd, t_params *params);
int dns_lookup(char *host, t_params *params);
char *ip_a_dns(const char *ip_str);
// int ping_check_flags(int argc, char **argv,active_flags *flags,t_params *params);

//AUX FUNCTIONS
double time_diff_ms(struct timeval *start, struct timeval *end);
unsigned short checksum(char *b, int len);
void hex_dump(const void *data , ssize_t total_size,struct iphdr *ip_hdr, struct icmphdr *inner_icmp,  t_params *params);
// void calculate_stats(t_packet *pings, t_params *param);


//FLAGS FUNCTIONS
t_flag *flags_config (void);
void test_flag(t_flag *flag,t_params *params);

//PARAMS
t_params *get_params (int argc, char **argv);

//DEBUG
void debug_params(t_params *params);

#endif