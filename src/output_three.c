/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_three.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/22/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/22/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ouput_filetype(struct stat *st)
{
	int filetype;

	filetype = st->st_mode & S_IFMT;
	if (filetype == S_IFREG)
		write(1, "-", 1);
	else if (filetype == S_IFBLK)
		write(1, "b", 1);
	else if (filetype == S_IFCHR)
		write(1, "c", 1);
	else if (filetype == S_IFDIR)
		write(1, "d", 1);
	else if (filetype == S_IFIFO)
		write(1, "p", 1);
	else if (filetype == S_IFLNK)
		write(1, "l", 1);
	else if (filetype == S_IFSOCK)
		write(1, "s", 1);
	else
		write(1, "-", 1);
}

void	output_extended(char *path)
{
	acl_t		acl;
	acl_entry_t	acl_e;
	ssize_t		xattr;

	acl = NULL;
	xattr = 0;
	acl = acl_get_link_np(path, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &acl_e) == -1)
	{
		acl_free(acl);
		acl = NULL;
	}
	xattr = listxattr(path, NULL, 0, XATTR_NOFOLLOW);
	if (xattr > 0)
		write (1, "@", 1);
	else if (acl != NULL)
		write(1, "+", 1);
	else
		write(1, " ", 1);
}

void	output_premissions(struct stat *st, char *path)
{
	ouput_filetype(st);
	(st->st_mode & S_IRUSR ? write(1, "r", 1) : write(1, "-", 1));
	(st->st_mode & S_IWUSR ? write(1, "w", 1) : write(1, "-", 1));
	(st->st_mode & S_IXUSR ? write(1, "x", 1) : write(1, "-", 1));
	(st->st_mode & S_IRGRP ? write(1, "r", 1) : write(1, "-", 1));
	(st->st_mode & S_IWGRP ? write(1, "w", 1) : write(1, "-", 1));
	(st->st_mode & S_IXGRP ? write(1, "x", 1) : write(1, "-", 1));
	(st->st_mode & S_IROTH ? write(1, "r", 1) : write(1, "-", 1));
	(st->st_mode & S_IWOTH ? write(1, "w", 1) : write(1, "-", 1));
	(st->st_mode & S_IXOTH ? write(1, "x", 1) : write(1, "-", 1));
	output_extended(path);
}

void	output_user_group_names(t_format *format, struct stat *st)
{
	struct group	*grp;
	struct passwd	*pd;

	pd = getpwuid(st->st_uid);
	grp = getgrgid(st->st_gid);
	ft_printf("%-*s  ", format->user_min_wid, pd->pw_name);
	ft_printf("%-*s ", format->group_min_wid, grp->gr_name);
}
