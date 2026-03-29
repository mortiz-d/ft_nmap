/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualice_binary_tree.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mortiz-d <mortiz-d@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 20:04:29 by mortiz-d          #+#    #+#             */
/*   Updated: 2025/09/23 23:30:04 by mortiz-d         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/libft.h"

void ft_binarytree_visualize(t_binary_tree* tree) {
    if (tree) {
        ft_binarytree_visualize(tree->left);
        printf("%d ( %d ) \n", tree->content, tree->count);
        ft_binarytree_visualize(tree->right);
    }
}
