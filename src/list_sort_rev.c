/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_sort_rev.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/21/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/21/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static int	compare_dates_rev(t_fileinfo *new_file, t_fileinfo *tmp)
{
	if (!new_file->st || !tmp->st)
		return (1);
	if (new_file->st->st_mtime > tmp->st->st_mtime)
		return (1);
	if (new_file->st->st_mtime == tmp->st->st_mtime &&
		ft_strcmp(tmp->filename, new_file->filename) > 0)
		return (1);
	return (0);
}

t_fileinfo	*sort_modtime_rev(t_fileinfo **files, t_fileinfo *new_file)
{
	t_fileinfo	*tmp;
	t_fileinfo	*head;
	t_fileinfo	*prev;

	tmp = *files;
	head = *files;
	prev = NULL;
	if (!*files)
		return (new_file);
	while (tmp && compare_dates_rev(new_file, tmp) > 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!prev)
	{
		head = new_file;
		new_file->next = tmp;
	}
	else
	{
		prev->next = new_file;
		new_file->next = tmp;
	}
	return (head);
}

t_fileinfo	*sort_reverse(t_fileinfo **files, t_fileinfo *new_file)
{
	t_fileinfo	*head;
	t_fileinfo	*tmp;
	t_fileinfo	*prev;

	tmp = *files;
	head = *files;
	prev = NULL;
	if (!*files)
		return (new_file);
	while (tmp && ft_strcmp(tmp->filename, new_file->filename) > 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!prev)
	{
		new_file->next = tmp;
		head = new_file;
	}
	else
	{
		prev->next = new_file;
		new_file->next = tmp;
	}
	return (head);
}
