#include "../lib/nmap.h"
#include <unistd.h>

void print_datetime_now(void) {
    struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);

        struct tm *local = localtime(&ts.tv_sec);

        char buffer[64];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", local);

        long milliseconds = ts.tv_nsec / 1000000;

        printf("%s.%03ld\n", buffer, milliseconds);
}

int free_all(t_list *flags, t_params *params)
{
    // ft_lstiter(flags,print_flag);
    free_flags(&flags);
    free_params(params);
    return 1;
}

int main(int argc, char **argv)
{
    t_list *flags = NULL;
    t_params *params = NULL;
    struct timespec begin, end;

    if (getuid()){
        printf("This program must be run as sudo.\n");
        return 0;
    }

    flags = flags_config();
    params =  params_default_config();

    if (!process_flags(argc, argv, flags,params))
    {
        return free_all(flags, params);
    }

    generate_result_table(params);
    debug_params(params);
    if (params->help == 1)
    {
        return free_all(flags, params);
    }

    if (params->ip_list == NULL)
    {
        printf("Error :No IPs where given to nmap\n");
        return free_all(flags, params);
    }

    clock_gettime(CLOCK_MONOTONIC, &begin);
    main_scan_logic(params);
    clock_gettime(CLOCK_MONOTONIC, &end);


    double elapsed = (end.tv_sec - begin.tv_sec) + (end.tv_nsec - begin.tv_nsec) / 1e9;
    printf("Performed scans in %.2f seconds\n", elapsed);

    print_result_table(params);
    return free_all(flags, params);
}
