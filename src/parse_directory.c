/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_directory.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/22/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/22/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static t_to_ls	*add_new_sorted(t_to_ls **to_ls, t_to_ls *new)
{
	t_to_ls		*head;
	t_to_ls		*tmp;
	t_to_ls		*prev;

	head = *to_ls;
	tmp = *to_ls;
	prev = tmp;
	while (tmp && ft_strcmp(new->name, tmp->name) > 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!tmp || !ft_strequ(tmp->name, prev->name))
	{
		new->next = tmp;
		prev->next = new;
	}
	else
	{
		new->next = tmp;
		head = new;
	}
	return (head);
}

static t_to_ls	*add_new_rev(t_to_ls **to_ls, t_to_ls *new)
{
	t_to_ls		*head;
	t_to_ls		*tmp;
	t_to_ls		*prev;

	head = *to_ls;
	tmp = *to_ls;
	prev = tmp;
	while (tmp && ft_strcmp(new->name, tmp->name) < 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!tmp || !ft_strequ(tmp->name, prev->name))
	{
		new->next = tmp;
		prev->next = new;
	}
	else
	{
		new->next = tmp;
		head = new;
	}
	return (head);
}

void			parse_directory(char *file, t_to_ls	**to_ls, unsigned int rev)
{
	t_to_ls		*new;

	new = (t_to_ls*)ft_memalloc(sizeof(t_to_ls));
	new->name = ft_strdup(file);
	new->next = NULL;
	if (!(*to_ls))
		*to_ls = new;
	else
		*to_ls = (!rev ? add_new_sorted(to_ls, new) : add_new_rev(to_ls, new));
}
