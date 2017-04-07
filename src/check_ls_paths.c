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
		return (*to_ls);
	if (ft_strequ(bad_path->name, head->name))
		head = head->next;
	else
	{
		prev = head;
		while (!ft_strequ(bad_path->name, prev->next->name))
			prev = prev->next;
		prev->next = bad_path->next;
	}
	free(bad_path->name);
	free(bad_path->path);
	free(bad_path);
	bad_path = NULL;
	return (head);
}

static void		new_single(t_single **singles, char *filename)
{
	t_single	*tmp;
	t_single	*new;

	new = (t_single*)ft_memalloc(sizeof(t_single));
	if (!new)
		return ;
	new->next = NULL;
	new->name = ft_strdup(filename);
	tmp = *singles;
	if (!*singles)
		*singles = new;
	else
	{
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

void			check_ls_paths(t_to_ls **to_ls, t_single **singles)
{
	t_to_ls		*tmp;
	t_to_ls		*next;
	DIR			*d;

	tmp = *to_ls;
	while (tmp)
	{
		next = tmp->next;
		if (!(d = opendir(tmp->name)))
		{
			if (errno == 20)
				new_single(singles, tmp->name);
			else
				ft_printf("ft_ls: %s: %s\n", tmp->name, strerror(errno));
			*to_ls = bad_path(to_ls, tmp);
		}
		else
			closedir(d);
		tmp = next;
	}
}
