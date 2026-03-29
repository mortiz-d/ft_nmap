/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/22 08:53:26 by mortiz-d          #+#    #+#             */
/*   Updated: 2021/11/30 17:34:43 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib/libft.h"

static int	argument_managment(int cont, const char *c, va_list argument);// void *var);
static int	print_format(int cont, const char *c, va_list argument);

int	ft_printf(const char *c, ...)
{
	int		cont;
	int		retorno;
	va_list	argument;

	cont = 0;
	retorno = 0;
	va_start(argument, c);
	retorno = print_format(cont, c, argument);
	va_end(argument);
	return (retorno);
}

int is_exact_word(char *str, char *cmp)
{
	if (ft_strncmp(str,cmp,ft_strlen(cmp)) == 0 && ft_strlen(str) == ft_strlen(cmp))
		return 1;
	return 0;
}

int is_key_word(char * str)
{
	char *aux[] = {"c","s","p","d","i","u","x","X","zu","02x","04x","%"};
	int i;

	i = 0;
	while (i <  12)
		if (is_exact_word(str,aux[i++]))
	 		return 1;
	return 0;
}

int check_argument(int cont, const char *c, int type)
{
	char keyword[4];

	if (c[cont + 1] == '\0')
		return 0;
	
	cont++;
	ft_memset(keyword,0,4);
	for (int i = 0; i < 4 ;i++)
	{
		if(c[cont + i] == '\0' || c[cont + i] == '\n' || c[cont + i] == ' '|| c[cont + i] == '\t'  )
			break;
		if (c[cont + i] == '%')
		{
			keyword[i] = c[cont + i];
			break;
		}
		keyword[i] = c[cont + i];
	}

	if(type == 0)
		return (is_key_word(keyword));
	
	if (is_key_word(keyword))
		return (ft_strlen(keyword));
	return (0);
	
}

static int	print_format(int cont, const char *c, va_list argument)
{
	int	retorno;

	retorno = 0;
	while (c[cont] != 0)
	{
		if (c[cont] == '%')
		{
			if (check_argument(cont,c,0))
				retorno += argument_managment(cont, c, argument );//va_arg(argument, void *));
			else
				retorno += ft_write_char(c[cont]);//retorno += argument_managment(cont, c, NULL);
			cont+=check_argument(cont,c,1);
		}	
		else
			retorno += ft_write_char(c[cont]);
		cont++;
	}
	return (retorno);
}

static int	argument_managment(int cont, const char *c, va_list argument) //void *var) 
{
	char keyword[4];

	if (c[cont + 1] == '\0')
		return 0;
	
	cont++;
	ft_memset(keyword,0,4);
	for (int i = 0; i < 4 ;i++)
	{
		if (c[cont + i] == '%')
		{
			keyword[i] = c[cont + i];
			break;
		}
		if(c[cont + i] == '\0' || c[cont + i] == '\n' || c[cont + i] == ' '|| c[cont + i] == '\t'  )
			break;
		keyword[i] = c[cont + i];
	}

	if (is_exact_word(keyword,"c"))								//CHARACTER
		return (ft_write_char (va_arg(argument, int)));
	else if (is_exact_word(keyword,"zu"))						//SIZE_T
		return (ft_write_size_t(va_arg(argument, size_t)));
	else if (is_exact_word(keyword,"s"))						//STRING
		return (ft_write_string(va_arg(argument, char *)));
	else if (is_exact_word(keyword,"p"))						//POINTER
		return (ft_write_pointer(va_arg(argument, void *)));
	else if (is_exact_word(keyword,"d") || is_exact_word(keyword,"i") )	//DECIMAL && //INTEGER
		return (ft_write_decimal(va_arg(argument, int )));
	else if (is_exact_word(keyword,"u"))						//UNSIGNED INTEGER
		return (ft_write_unsigned_integer(va_arg(argument, unsigned int)));
	else if (is_exact_word(keyword,"x"))						//HEXADECIMAL MIN
		return (ft_write_hexadecimal(va_arg(argument, int),"0123456789abcdef"));
	else if (is_exact_word(keyword,"04x"))						//HEXADECIMAL MIN
		return (ft_write_hexadecimal_XX(va_arg(argument, unsigned int),"0123456789abcdef",1,4,'0'));
	else if (is_exact_word(keyword,"02x"))						//HEXADECIMAL MIN
		return (ft_write_hexadecimal_XX(va_arg(argument, unsigned int),"0123456789abcdef",1,2,'0'));
	else if (is_exact_word(keyword,"X"))						//HEXADECIMAL MAX
		return (ft_write_hexadecimal(va_arg(argument, int),"0123456789ABCDEF"));
	else
		return (ft_write_char('%'));

	// if (is_exact_word(keyword,"c"))								//CHARACTER
	// 	return (ft_write_char (va_arg(argument, int)));
	// else if (is_exact_word(keyword,"zu"))						//SIZE_T
	// 	return (ft_write_size_t(va_arg(argument, size_t)));
	// else if (is_exact_word(keyword,"s"))						//STRING
	// 	return (ft_write_string(va_arg(argument, char *)));
	// else if (is_exact_word(keyword,"p"))						//POINTER
	// 	return (ft_write_pointer(va_arg(argument, void *)));
	// else if (is_exact_word(keyword,"d"))						//DECIMAL
	// 	return (ft_write_decimal(va_arg(argument, int )));
	// else if (is_exact_word(keyword,"i"))						//INTEGER
	// 	return (ft_write_decimal(va_arg(argument, int)));
	// else if (is_exact_word(keyword,"u"))						//UNSIGNED INTEGER
	// 	return (ft_write_unsigned_integer(va_arg(argument, unsigned int)));
	// else if (is_exact_word(keyword,"x"))						//HEXADECIMAL MIN
	// 	return (ft_write_hexadecimal_min(va_arg(argument, int)));
	// else if (is_exact_word(keyword,"X"))						//HEXADECIMAL MAX
	// 	return (ft_write_hexadecimal_max(va_arg(argument, int)));
	// else
	// 	return (ft_write_char('%'));
}

// static int	argument_managment(int cont, const char *c, va_list argument) //, void *var)
// {

// 	if (c[cont + 1] == 'c')
// 		return (ft_write_char (va_arg(argument, int)));
// 	else if (c[cont + 1] == 'z')
// 		return (ft_write_size_t(va_arg(argument, size_t)));
// 	else if (c[cont + 1] == 's')
// 		return (ft_write_string(va_arg(argument, char *)));
// 	else if (c[cont + 1] == 'p')
// 		return (ft_write_pointer(va_arg(argument, void *)));
// 	else if (c[cont + 1] == 'd')
// 		return (ft_write_decimal(va_arg(argument, int )));
// 	else if (c[cont + 1] == 'i')
// 		return (ft_write_integer(va_arg(argument, int)));
// 	else if (c[cont + 1] == 'u')
// 		return (ft_write_unsigned_integer(va_arg(argument, unsigned int)));
// 	else if (c[cont + 1] == 'x')
// 		return (ft_write_hexadecimal_min(va_arg(argument, int)));
// 	else if (c[cont + 1] == 'X')
// 		return (ft_write_hexadecimal_max(va_arg(argument, int)));
// 	else
// 		return (ft_write_char('%'));
// }
