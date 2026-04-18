

#include "../lib/libft.h"

int ft_isnumber(char *str)
{
    int i = 0;

    if (!str || !str[0])
        return 0;

    if (str[i] == '-' || str[i] == '+')
       i++;

    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return 0;
        i++;
    }
    return 1;
}