#include "../lib/nmap.h"

int get_local_ip(char *dest_ip, char *out_ip)
{
    int sock;
    struct sockaddr_in dest;
    struct sockaddr_in local;
    socklen_t len = sizeof(local);

    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(80);
    inet_pton(AF_INET, dest_ip, &dest.sin_addr);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        return 0;

    if (connect(sock, (struct sockaddr *)&dest, sizeof(dest)) < 0)
    {
        close(sock);
        return 0;
    }

    if (getsockname(sock, (struct sockaddr *)&local, &len) < 0)
    {
        close(sock);
        return 0;
    }

    inet_ntop(AF_INET, &local.sin_addr, out_ip, INET_ADDRSTRLEN);

    close(sock);
    return 1;
}

char *dns_lookup(char *host) {
    struct addrinfo hints;
    struct addrinfo *result;
	struct sockaddr_in *addr;
    int status;
    char *res;

    memset(&hints, 0, sizeof(hints));
    res = ft_calloc(INET_ADDRSTRLEN, 1);
    hints.ai_family = AF_INET;                       // Para que nos devuelva en ipv4
    hints.ai_socktype = SOCK_STREAM; // protocolo tcp
    hints.ai_flags = AI_CANONNAME | AI_ADDRCONFIG;	 //AI_CANONNAME para poner el nombre completo del host y AI_ADDRCONFIG para especificar ips deseadas (en este caso ipv4)

    status = getaddrinfo(host, NULL, &hints, &result);
    if (status != 0) {
        free(res);
		if (DEBUG)
        	fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        return NULL;
    }
    addr = (struct sockaddr_in *)result->ai_addr;
    
    inet_ntop(AF_INET, &(addr->sin_addr), res, INET_ADDRSTRLEN);
    freeaddrinfo(result);
    return res;
}