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

void	output_date(t_fileinfo *file)
{
	time_t	curtime;
	char	*filetime;
	char	*p2;
	int 	time_or_year;

	time_or_year = 0;
	curtime = time(NULL);
	filetime = ft_strsub(ctime(&file->st->st_mtime), 4, 6);
	if (!filetime)
		return ;
	if (file->st->st_mtime > curtime || file->st->st_mtime + SIX_MONTHS < curtime)
		p2 = ft_strsub(ctime(&file->st->st_mtime), 20, 4);
	else
		p2 = ft_strsub(ctime(&file->st->st_mtime), 11, 5);
	ft_printf("%s %5s ", filetime, p2);
	if (filetime)
		free(filetime);
	if (p2)
		free(p2);
}

void	output_file_symbol(t_fileinfo *file)
{
	if ((file->st->st_mode & S_IFMT) == S_IFDIR)
		write(1, "/", 1);
	else if ((file->st->st_mode & S_IFMT) == S_IFLNK)
		write(1, "@", 1);
	else if ((file->st->st_mode & S_IFMT) == S_IFSOCK)
		write(1, "=", 1);
	else if (!file->filename || file->filename == '\0')
		write(1, "%", 1);
	else if ((file->st->st_mode & S_IFMT) == S_IFIFO)
		write(1, "|", 1);
	else if (file->st->st_mode & S_IXUSR ||
		file->st->st_mode & S_IXGRP || file->st->st_mode & S_IXOTH)
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

// void	output_name(t_fileinfo *file, t_options *options)
// {
// 	if (file->rights[0] == 'l')
// 	{
// 		output a link
// 	}
// }

void	ouput_filetype(t_fileinfo *file)
{
	int filetype;

	filetype = file->st->st_mode & S_IFMT;
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

void	output_extended(t_fileinfo *file)
{
	acl_t		acl;
	acl_entry_t	acl_e;
	ssize_t		xattr;

	acl = NULL;
	xattr = 0;
	acl = acl_get_link_np(file->path, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &acl_e) == -1)
	{
		acl_free(acl);
		acl = NULL;
	}
	xattr = listxattr(file->path, NULL, 0, XATTR_NOFOLLOW);
	if (xattr > 0)
		write (1, "@", 1);
	else if (acl != NULL)
		write(1, "+", 1);
	else
		write(1, " ", 1);
}

void	output_premissions(t_fileinfo *file)
{
	ouput_filetype(file);
	(file->st->st_mode & S_IRUSR ? write(1, "r", 1) : write(1, "-", 1));
	(file->st->st_mode & S_IWUSR ? write(1, "w", 1) : write(1, "-", 1));
	(file->st->st_mode & S_IXUSR ? write(1, "x", 1) : write(1, "-", 1));
	(file->st->st_mode & S_IRGRP ? write(1, "r", 1) : write(1, "-", 1));
	(file->st->st_mode & S_IWGRP ? write(1, "w", 1) : write(1, "-", 1));
	(file->st->st_mode & S_IXGRP ? write(1, "x", 1) : write(1, "-", 1));
	(file->st->st_mode & S_IROTH ? write(1, "r", 1) : write(1, "-", 1));
	(file->st->st_mode & S_IWOTH ? write(1, "w", 1) : write(1, "-", 1));
	(file->st->st_mode & S_IXOTH ? write(1, "x", 1) : write(1, "-", 1));
	output_extended(file);

}

void	output_user_group_names(t_format *format, t_fileinfo *file)
{
	struct group	*grp;
	struct passwd	*pd;

	pd = getpwuid(file->st->st_uid);
	grp = getgrgid(file->st->st_gid);
	ft_printf("%*s  ", format->user_min_wid, pd->pw_name);
	ft_printf("%*s ", format->group_min_wid, grp->gr_name);
}

void	follow_links(t_fileinfo *file)
{
	char	link_buf[1024];
	ssize_t	count;

	ft_bzero(link_buf, sizeof(link_buf));
	count = readlink(file->path, link_buf, sizeof(link_buf));
	if (count < 0)
	{
		perror("readlinkl");
		exit(EXIT_FAILURE);
	}
	link_buf[count] = '\0';
	ft_printf(" -> %s", link_buf);
}

void	output_size_or_sys(t_format *format, t_fileinfo *file)
{
	if ((file->st->st_mode & S_IFMT) == S_IFLNK ||
		(file->st->st_mode & S_IFMT) == S_IFCHR ||
		(file->st->st_mode & S_IFMT) == S_IFBLK)
	{
		if ((file->st->st_mode & S_IFMT) == S_IFLNK)
			ft_printf("%*s%*s", format->rdev_size, "", format->rdev2_size, "");
		else
			ft_printf("  %*d,", format->rdev_size, file->st->st_rdev >> 24);
		ft_printf(" %*d ", format->rdev2_size, file->st->st_rdev & 0xFFFFFF);
	}
	else
	{
		if (format->rdev_size > 0 || format->rdev2_size > 0)
			ft_printf("  %*s%*s", format->rdev_size, "", format->rdev2_size, "");
		ft_printf(" %*lld ", format->file_size, file->st->st_size);
	}
}

void	output_info(t_to_ls *directory, t_all *all)
{
	t_fileinfo *tmp;

	tmp = all->files;
	if (all->print_dir)
		ft_printf("%s:\n", directory->name);
	if (all->options->option_l)
		print_blocksize(all->files, all->options);
	// if (all->options->option_G)
	// 	output_in_color(tmp, all, directory); // make this
	// else all this VV
	while (tmp)
	{
		if (all->options->option_i)
			ft_printf("%*llu ", all->format->serial_min_wid ,tmp->st->st_ino);
		if (all->options->option_l)
		{
			output_premissions(tmp); // done
			ft_printf(" %*d ", all->format->links_min_wid, tmp->st->st_nlink);
			if (!all->options->option_n)
				output_user_group_names(all->format, tmp); // done
			else
				ft_printf("%*d  %*d ",  all->format->user_min_wid, tmp->st->st_uid, all->format->group_min_wid, tmp->st->st_gid);
			output_size_or_sys(all->format, tmp);
			output_date(tmp);
		}
		ft_printf("%s", tmp->filename);
		if (all->options->option_F)
			output_file_symbol(tmp);
		if (((tmp->st->st_mode & S_IFMT) == S_IFLNK))
			follow_links(tmp);
		ft_printf("\n");
		tmp = tmp->next;
	}
}
