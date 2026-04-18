#include "../lib/nmap.h"

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

char *ip_a_dns(const char *ip_str)
{
    struct sockaddr_in sa;
    char host[NI_MAXHOST];
    int err;
    char *dns;

    ft_memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;

    if (inet_pton(AF_INET, ip_str, &sa.sin_addr) != 1)
        return NULL;

    err = getnameinfo((struct sockaddr *)&sa, sizeof sa, host, sizeof host, NULL, 0, NI_NAMEREQD);
    if (err != 0)
        return NULL;

    dns = malloc(strlen(host) + 1);
    if (!dns)
        return NULL;

    strcpy(dns, host);
    return dns;
}