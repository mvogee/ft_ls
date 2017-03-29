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

void			check_ls_paths(t_to_ls **to_ls) // this clears file paths >;(
{
	t_to_ls		*tmp;
	t_to_ls		*next;
	DIR			*d;

	tmp = *to_ls;
	while (tmp)
	{
		d = opendir(tmp->name);
		next = tmp->next;
		if (!d)
		{
			if (errno == 20)
				tmp->ls_file = 1;
			else
			{
				ft_printf("ft_ls: %s: %s\n", tmp->name, strerror(errno));
				*to_ls = bad_path(to_ls, tmp);
			}
		}
		else
			free(d);
		tmp = next;
	}
	if (!(*to_ls))
		exit(EXIT_FAILURE);
}
