//#include <studio.h>
#include "../lib/nmap.h"

int main(int argc, char **argv) 
{  
    (void) argv;
    t_params *params;
    int i;

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

    free_params(params);
    
    return 0;
}