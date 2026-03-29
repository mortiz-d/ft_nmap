
#include "../lib/libft.h"

int ft_is_exact_word(char *str, char *cmp)
{
	if (ft_strncmp(str,cmp,ft_strlen(cmp)) == 0 && ft_strlen(str) == ft_strlen(cmp))
		return 1;
	return 0;
}