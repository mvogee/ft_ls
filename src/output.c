/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/22/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/22/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	follow_links(char *filepath)
{
	char			link_buf[1024];
	ssize_t			count;

	ft_bzero(link_buf, sizeof(link_buf));
	count = readlink(filepath, link_buf, sizeof(link_buf));
	if (count < 0)
	{
		perror("readlinkl");
		exit(EXIT_FAILURE);
	}
	link_buf[count] = '\0';
	ft_printf(" -> %s", link_buf);
}

void	output_size_or_sys(t_format *format, struct stat *st)
{
	if ((st->st_mode & S_IFMT) == S_IFCHR || (st->st_mode & S_IFMT) == S_IFBLK)
	{
		ft_printf("  %*d,", format->rdev_size, st->st_rdev >> 24);
		ft_printf(" %*d ", format->rdev2_size, st->st_rdev & 0xFFFFFF);
	}
	else
	{
		if (format->rdev_size > 0 || format->rdev2_size > 0)
			ft_printf("  %*s%*s", format->rdev_size, "",
									format->rdev2_size, "");
		ft_printf(" %*lld ", format->file_size, st->st_size);
	}
}

void	extended_output_l(t_fileinfo *tmp, t_all *all)
{
	output_premissions(tmp->st, tmp->path);
	ft_printf(" %*d ", all->format->links_min_wid, tmp->st->st_nlink);
	if (!all->options->option_n)
		output_user_group_names(all->format, tmp->st);
	else
		ft_printf("%*d  %*d ", all->format->user_min_wid, tmp->st->st_uid,
							all->format->group_min_wid, tmp->st->st_gid);
	output_size_or_sys(all->format, tmp->st);
	output_date(tmp->st);
}

void	output_info(t_to_ls *directory, t_all *all)
{
	t_fileinfo		*tmp;

	tmp = all->files;
	if (all->print_dir && !(ft_strequ(directory->name, "./") ||
								ft_strequ(directory->name, ".")))
		ft_printf("%s:\n", directory->name);
	if (all->options->option_l)
		print_blocksize(all->files, all->options);
	while (tmp)
	{
		if (all->options->option_i)
			ft_printf("%*llu ", all->format->serial_min_wid, tmp->st->st_ino);
		if (all->options->option_l)
			extended_output_l(tmp, all);
		ft_printf("%s", tmp->filename);
		if (all->options->option_up_f)
			output_file_symbol(tmp->st, tmp->filename);
		if (((tmp->st->st_mode & S_IFMT) == S_IFLNK) && all->options->option_l)
			follow_links(tmp->path);
		tmp = tmp->next;
		ft_printf("\n");
	}
}
