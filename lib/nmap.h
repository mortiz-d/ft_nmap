#ifndef NMAP_H
# define NMAP_H

#define _POSIX_C_SOURCE 200809L //Esto es para mi ordenador que puede actuar funny el hijopu
#define NMAP_USSAGE_ERROR "./ft_nmap --help <args>\n"

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

//FLAGS FUNCTIONS
t_flag *flags_config (void);
void test_flag(t_flag *flag,t_params *params);

//PARAMS
t_params *get_params (int argc, char **argv);
void free_params(t_params *params);

//DEFAULT CONFIG
t_list **default_ports(void);

//DEBUG
void debug_params(t_params *params);

#endif