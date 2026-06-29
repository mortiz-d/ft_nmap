#include "../../lib/nmap.h"

void print_result_table(t_list *lst)
{
    t_result_port *s;

    printf("\n");
    printf("+--------+------------+------------+------------+------------+------------+------------+\n");
    printf("| PORT   | SYN        | NULL       | FIN        | XMAS       | ACK        | UDP        |\n");
    printf("+--------+------------+------------+------------+------------+------------+------------+\n");

    while (lst)
    {
        s = (t_result_port *)lst->content;

        printf("| %-6d | %-10s | %-10s | %-10s | %-10s | %-10s | %-10s |\n",s->port_nbr,port_state_str(s->syn),port_state_str(s->nul),port_state_str(s->fin),port_state_str(s->xmas),port_state_str(s->ack),port_state_str(s->udp));
        lst = lst->next;
    }

    printf("+--------+------------+------------+------------+------------+------------+------------+\n");
}