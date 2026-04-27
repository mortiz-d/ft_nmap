#include "../lib/nmap.h"

void fill_packet_tcp(t_packet *pck, int port){
    ft_bzero(pck, sizeof(*pck));
    pck->header.uh_dport = htons(port);
    pck->header.uh_ulen  = htons(sizeof(*pck));
    pck->header.uh_flags = TH_SYN;
    pck->header.syn = 1;
    pck->header.ack = 0;
    // pck->header.uh_sum = calculate_checksum(pck, sizeof(*pck));
}


void *scan_port(void *args){
    //castear args a lo que sea
    //hacer el scan
    return NULL;
}

void main_scan_logic(t_params* args){
    pthread_t thread;

    for(int i = 0; i < args->threads; ++i){
        pthread_create(&thread, NULL, scan_port, &arg);
    }
}
