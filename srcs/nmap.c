#include "../lib/nmap.h"


int nmap(t_params *params,char * str)
{
    (void)params;
    (void)str;
    int sockfd;


    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    
    if (sockfd < 0) {
        fprintf(stderr, "Crap");
        return 0;
    }
    
    close(sockfd);
    return 1;
}