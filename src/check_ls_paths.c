/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_ls_paths.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/23/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/23/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static t_to_ls	*bad_path(t_to_ls **to_ls, t_to_ls *bad_path)
{
	t_to_ls		*prev;
	t_to_ls		*head;

	head = *to_ls;
	if (!bad_path)
		return (NULL);
	if (ft_strequ(bad_path->name, head->name))
		head = head->next;
	else
	{
		prev = head;
		while (!ft_strequ(bad_path->name ,prev->next->name))
			prev = prev->next;
		prev->next = bad_path->next;
	}
	free(bad_path);
	return (head);
}

void			check_ls_paths(t_to_ls **to_ls)
{
	t_to_ls		*tmp;
	t_to_ls		*next;
	DIR			*d;

	tmp = *to_ls;
	while (tmp)
	{
		d = opendir(tmp->name);
		if (!d)
		{
			next = tmp->next;
			write(2, "ft_ls: ", 8);
			write(2, tmp->name, ft_strlen(tmp->name) + 1);
			write(2, ": No such file or directory\n", 29);
			*to_ls = bad_path(to_ls, tmp);
			tmp = next;
		}
		else
		{
			free(d);
			tmp = tmp->next;
		}
	}
	if (!(*to_ls))
		exit(EXIT_FAILURE);
}
