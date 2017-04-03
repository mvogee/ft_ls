/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   freeing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/21/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/21/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	free_files(t_fileinfo **files)
{
	t_fileinfo		*tmp;
	t_fileinfo		*next;

	tmp = *files;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->st);
		free(tmp->path);
		free(tmp->filename);
		free(tmp);
		tmp = next;
	}
	*files = NULL;
}

void	free_to_ls(t_to_ls **to_ls)
{
	t_to_ls		*tmp;
	t_to_ls		*next;
	
	tmp = *to_ls;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->name);
		free(tmp->path);
		free(tmp);
		tmp = next;
	}
}
