/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_flag.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 13:20:50 by mortiz-d          #+#    #+#             */
/*   Updated: 2026/04/10 19:08:51 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft.h"
#include "../../lib/flag.h"

void print_flag(void *content)
{
    t_flag *f = (t_flag *)content;
    if (!f)
        return;

    printf("Flag: %s\n", f->name);
    printf("  Type: %d\n", f->type);
    printf("  Active: %s\n", f->active ? "YES" : "NO");

    printf("  Apply func: %p\n", (void *)f->apply);
    printf("  Not apply func: %p\n", (void *)f->not_apply);

    if (!f->active)
    {
        printf("  Value: (not set)\n\n");
        return;
    }

    printf("  Value: ");

    if (f->type == FLAG_INTEGER)
        printf("%d", f->value.int_value);

    else if (f->type == FLAG_RANGE_INTEGER)
        printf("min=%d max=%d",
               f->value.range.min,
               f->value.range.max);

    else if (f->type == FLAG_STRING)
        printf("%s", f->value.str_value);

    else if (f->type == FLAG_CHAR)
        printf("%c", f->value.char_value);

    else if (f->type == FLAG_BOOLEAN)
        printf("%s", f->value.bool_value ? "true" : "false");

    else if (f->type == FLAG_NONE)
        printf("(no value)");

    else if (f->type == FLAG_HELP)
        printf("(help flag)");

    printf("\n\n");
}
