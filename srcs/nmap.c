#include "../lib/nmap.h"


int nmap(t_params *params,char * ip)
{
    (void)ip;
    int sockfd;
    int n_ports_per_thread = params->n_ports / params->threads;
    //En esta funcion deberiamos de crear los numeros de hilos en relacion con los puertos y el ultimo hilo que se quede con el sobrante
    printf("%i/%i = %i\n", params->n_ports , params->threads ,n_ports_per_thread);
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    
    if (sockfd < 0) {
        fprintf(stderr, "Crap\n");
        return 0;
    }
    
    close(sockfd);
    return 1;
}