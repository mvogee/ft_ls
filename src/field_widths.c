/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   field_widths.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/21/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/21/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


static int	owner_width(struct stat *st, t_format *format)
{
	char			*owner;
	struct passwd	*pd;
	int				width;

	pd = getpwuid(st->st_uid);
	owner = pd->pw_name;
	width = ft_strlen(owner);
	if (width > format->user_min_wid)
		return (width);
	return (format->user_min_wid);
}

static int	group_width(struct stat *st, t_format *format)
{
	char			*group;
	struct group	*grp;
	int				width;

	grp = getgrgid(st->st_gid);
	group = grp->gr_name;
	width = ft_strlen(group);
	if (width > format->group_min_wid)
		return (width);
	return (format->group_min_wid);
}

void		field_widths(t_format *format, t_fileinfo *new_file)
{
	int				len;

	if ((len = ft_strlen(ft_lltoa((long long)new_file->st->st_ino))) > format->serial_min_wid)
		format->serial_min_wid = len;
	if ((len = ft_strlen(ft_itoa(new_file->st->st_nlink))) > format->links_min_wid)
		format->links_min_wid = len;
	if ((len = ft_strlen(ft_lltoa(new_file->st->st_size))) > format->file_size)
		format->file_size = len;
	format->user_min_wid = owner_width(new_file->st, format);
	format->group_min_wid = group_width(new_file->st, format);
	if ((new_file->st->st_mode & S_IFMT) == S_IFCHR ||
		(new_file->st->st_mode & S_IFMT) == S_IFBLK)
	{
		if ((len = ft_strlen(ft_itoa((new_file->st->st_rdev >> 24)))) > format->rdev_size)
			format->rdev_size = len;
		if ((len = ft_strlen(ft_itoa((new_file->st->st_rdev & 0xFFFFFF)))) > format->rdev2_size)
			format->rdev2_size = len;
	}
}
