#include "../lib/libft.h"

// void	alznum_s(size_t n)
// {	
// 	size_t	x;

// 	x = n % 10;
// 	if (n / 10 != 0)
// 		alznum_s(n / 10);
// 	ft_write_char(x + '0');
// }

void alznum_s(size_t n)
{
    if (n >= 10)
        alznum_s(n / 10);
    ft_write_char((n % 10) + '0');
}

int	ft_write_size_t(size_t number)
{
	int	cont;
	
	cont = 1;
	alznum_s(number);
	while (number / 10 != 0)
	{
		number /= 10;
		cont++;
	}
	return (cont);
}