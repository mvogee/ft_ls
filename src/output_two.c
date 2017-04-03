/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/22/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/22/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	output_date(struct stat *st)
{
	time_t	curtime;
	char	*filetime;
	char	*p2;
	int 	time_or_year;

	time_or_year = 0;
	curtime = time(NULL);
	filetime = ft_strsub(ctime(&st->st_mtime), 4, 6);
	if (!filetime)
		return ;
	if (st->st_mtime > curtime || st->st_mtime + SIX_MONTHS < curtime)
		p2 = ft_strsub(ctime(&st->st_mtime), 20, 4);
	else
		p2 = ft_strsub(ctime(&st->st_mtime), 11, 5);
	ft_printf("%s %5s ", filetime, p2);
	if (filetime)
		free(filetime);
	if (p2)
		free(p2);
}

void	output_file_symbol(struct stat *st, char *filename)
{
	if ((st->st_mode & S_IFMT) == S_IFDIR)
		write(1, "/", 1);
	else if ((st->st_mode & S_IFMT) == S_IFLNK)
		write(1, "@", 1);
	else if ((st->st_mode & S_IFMT) == S_IFSOCK)
		write(1, "=", 1);
	else if (!filename || *filename == '\0')
		write(1, "%", 1);
	else if ((st->st_mode & S_IFMT) == S_IFIFO)
		write(1, "|", 1);
	else if (st->st_mode & S_IXUSR ||
			st->st_mode & S_IXGRP || st->st_mode & S_IXOTH)
		write(1, "*", 1);
}

void	print_blocksize(t_fileinfo *files, t_options *options)
{
	t_fileinfo	*tmp;
	blkcnt_t	total;

	tmp = files;
	total = 0;
	while (tmp)
	{
		if (tmp->filename[0] == '.' && !options->option_a)
		{
			tmp = tmp->next;
			continue ;
		}
		total += tmp->st->st_blocks;
		tmp = tmp->next;
	}
	ft_printf("total %zu\n", total);
}
