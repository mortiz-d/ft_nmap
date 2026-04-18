//#include <studio.h>
#include "../lib/nmap.h"
#include <unistd.h>

int main(int argc, char **argv)
{
    (void) argv;
    t_params *params;
    int i;
    if (getuid()){
        dprintf (2, "This program must be run as root, quitting\n");
        return 0;
    }

    if (argc < 2)
    {
        printf (NMAP_USSAGE_ERROR);
        return 0;
    }

    params = get_params(argc,argv);
    debug_params(params);

    i = 1;
    while (i < argc)
    {
        printf("%i - %s\n",i,argv[i]);
        i++;
    }

    printf("Starting ft_nmap\n");
    //logica

    free_params(params);
    printf("ft_nmap done: %i IP address (%i host up) scanned in %.2f seconds", 1, 1, 1.23);
    return 0;
}
