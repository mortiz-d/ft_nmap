#include "../lib/nmap.h"


//FASES
    //CREAMOS EL SOCK
    //CREAMOS EL MENSAJE A MANDAR
    //MANDAMOS EL MENSAJE
    //RECIVIMOS EL MENSAJE
    //PROCESAMOS EL MENSAJE
//FIN
int scan_tcp(t_params *params, t_scan type, struct sockaddr_in addr, int port)
{
    char packet[4096];
    char buffer[4096];
    (void) params;
    (void) type;
    (void) addr;
    (void) port;
    (void) packet;
    (void) buffer;
    int sockfd;
    

    sockfd = socket_connection(params);
    if (!sockfd)
    {
        printf("Error: No se pudo crear el socket del puerto %i\n",port);
        return 0;
    }
    printf("Socket creado para puerto %i\n",port);
    // (char *packet, t_params *params, struct sockaddr_in addr, int port, t_scan type)
    build_packet(packet,params,addr,port,type);
    // build_packet(packet, params, addr, port, type);
    send_packet(sockfd, packet, addr);
    recv_packet(sockfd,buffer);
  
    close(sockfd);
    return 1;
}