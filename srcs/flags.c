#include "../lib/nmap.h"

static bool check_integer(char *str)
{
    char *aux_c;
    int aux_i;

    aux_i = ft_atoi(str);
    aux_c = ft_itoa(aux_i);
    if (!ft_strncmp(str, aux_c, ft_strlen(str)) == 0)
	{
		free(aux_c);
		return (false);
	}
    free(aux_c);
    return true;
}

static t_port* create_port (char *str)
{
    t_port  *ret;
    char    **ports = NULL;
    bool    create = false;
    int     count = 0;

    ret = ft_calloc(1,sizeof(t_port));
    if (ft_strchr(str,'-') != NULL)
    {
        ret->range = true;
        ports = ft_split(str,'-');
        while (ports[count])
            count++;
                
        if (count == 2)
        {
            if (check_integer(ports[0]) && check_integer(ports[1]))
            {
                create = true;
                ret->port_min = ft_atoi(ports[0]);
                ret->port_max = ft_atoi(ports[1]);
            }
        }
    }
    else
    {
        if (check_integer(str))
        {
            ret->port_nbr = ft_atoi(str);
            create = true;
        }
    }

    if (ports)
    {
        for(int i = 0; ports[i] != NULL; i++)
            free(ports[i]);
        free(ports);
    }

    if(!create)
    {
        free(ret);
        return NULL;
    }
    return ret;
}


static t_list **extract_ports(char* str)
{
    t_list **lst_ports = ft_calloc(1,sizeof(t_list*));
    char **ports = NULL ;
    t_port *aux = NULL;
    
    ports = ft_split(str,',');
    for(int i = 0; ports[i] != NULL; i++)
    {
        aux = create_port(ports[i]);
        if (aux)
            ft_lstadd_back(lst_ports,ft_lstnew(aux));
        else
        {
            ft_lstclear(lst_ports);
            free(lst_ports);
            lst_ports = NULL;
            break;
        }
    }

    for(int i = 0; ports[i] != NULL; i++)
        free(ports[i]);
    free(ports);

    return lst_ports;

}

static t_list **extract_filename_ips(char *filename)
{
    t_list **lst_ips = ft_calloc(1,sizeof(t_list*));
    char *aux;
    int fd;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
        return NULL;
    aux = get_next_line(fd);    
    while (aux)
    {
        ft_lstadd_back(lst_ips,ft_lstnew(aux));
        aux = get_next_line(fd);
    }
    close(fd);
    return lst_ips;

}

static t_scan get_scan(char *str)
{
    const char *scan_names[] = {"SYN","NUL","FIN","XMAS","ACK","UDP"};
    int num_scans = sizeof(scan_names) / sizeof(scan_names[0]);

    for (int i = 0; i < num_scans; i++)
    {
        if (strcmp(str, scan_names[i]) == 0)
            return (t_scan)i;
    }

    return SCAN_UNKNOWN;
}

static t_list **extract_scan(char *str)
{
    t_list **lst_scans = ft_calloc(1,sizeof(t_list*));
    char **scans = NULL;
    t_scan *new_aux;
    t_scan aux;
    
    scans = ft_split(str,'/');
    for(int i = 0; scans[i] != NULL; i++)
    {
        aux = get_scan(scans[i]);
        new_aux = malloc(sizeof(t_scan));
        if (!new_aux)
            return NULL;
        *new_aux = aux;
        ft_lstadd_back(lst_scans, ft_lstnew(new_aux));
    }

    for(int i = 0; scans[i] != NULL; i++)
        free(scans[i]);
    free(scans);

    return lst_scans;
}

void test_flag(t_flag *flag,t_params *params)
{
    if (check_flag_status(flag,"--help"))
        params->help = true;
    if (check_flag_status(flag,"--ports"))
        params->ports = extract_ports(get_flag_value(flag,"--ports")->s);
    if (check_flag_status(flag,"--ip"))
        params->destination = ft_strdup(get_flag_value(flag,"--ip")->s);
    if (check_flag_status(flag,"--file"))
        params->filename_ip = extract_filename_ips(get_flag_value(flag,"--file")->s);
    if (check_flag_status(flag,"--speedup"))
        params->threads = get_flag_value(flag,"--speedup")->i;
    if (check_flag_status(flag,"--scan"))
        params->scan = extract_scan(get_flag_value(flag,"--scan")->s);
}


t_flag *flags_config (void)
{
    t_flag *flag = NULL;

    flag = set_up_flag(flag,"--help",FLAG_NONE,0,0);
	flag = set_up_flag(flag,"--ports",FLAG_STRING,0,0);
    flag = set_up_flag(flag,"--ip",FLAG_STRING,0,0);
    flag = set_up_flag(flag,"--file",FLAG_STRING,0,0);
	flag = set_up_flag(flag,"--speedup",FLAG_INTEGER,0,255);
    flag = set_up_flag(flag,"--scan",FLAG_STRING,0,4);
    // visualize_flags(flag);
    return flag;
}


