/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readfile.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/09 13:47:09 by mortiz-d          #+#    #+#             */
/*   Updated: 2026/01/13 00:05:00 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../../lib/get_next_line.h"

char * get_text_file(char *filename)
{
    int fd;
    char *str,*aux,*line;

    fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf("Error : %s : no existe el archivo \n",filename);
        return NULL;
	}

	str = NULL;
	while ((line = get_next_line(fd)) != NULL)
    {
        if (!str)
            str = ft_strdup(line);
        else
        {
            aux = ft_strjoin(str, line);
            free(str);
            str = aux;
        }
        free(line);
    }
	
			
	close(fd);
    return (str);

}