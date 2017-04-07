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
	time_t		curtime;
	char		*filetime;
	char		*p2;
	int			time_or_year;

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

void	output_single_files(t_all *all, t_single **singles)
{
	t_single	*tmp;

	tmp = *singles;
	if (tmp)
	{
		while (tmp)
		{
			output_single_file(all, tmp->name);
			tmp = tmp->next;
		}
	}
}

void	output_single_file(t_all *all, char *file)
{
	struct stat		st;

	if (lstat(file, &st) == 0)
	{
		if (all->options->option_i)
			ft_printf("%*llu ", all->format->serial_min_wid, st.st_ino);
		if (all->options->option_l)
		{
			output_premissions(&st, file);
			ft_printf(" %*d ", all->format->links_min_wid, st.st_nlink);
			if (!all->options->option_n)
				output_user_group_names(all->format, &st);
			else
				ft_printf("%*d  %*d ", all->format->user_min_wid, st.st_uid,
										all->format->group_min_wid, st.st_gid);
			output_size_or_sys(all->format, &st);
			output_date(&st);
		}
		ft_printf("%s", file);
		if (all->options->option_up_f)
			output_file_symbol(&st, file);
		if (((st.st_mode & S_IFMT) == S_IFLNK))
			follow_links(file);
		ft_printf("\n");
	}
}
