/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/21/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/21/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	throw_error(char *bad_info)
{
	write(2, "ft_ls: ", 8);
	write(2, "illegal option -- ", 19);
	write(2, &bad_info[0], 1);
	write(2, "\nusage: ls [-aRFfGilrt] [file ...]\n", 36);
	exit(EXIT_FAILURE);
}

// \/___________ gets ____________ \/
int		owner_width(struct stat *st, t_format *format)
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

int		group_width(struct stat *st, t_format *format)
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
//    ^ _______ gets __________ ^

void		field_widths(t_format *format, t_fileinfo *new_file)
{
	int		len;

	if ((len = ft_strlen(ft_lltoa((long long)new_file->st->st_ino))) > format->serial_min_wid)
		format->serial_min_wid = len;
	if ((len = ft_strlen(ft_itoa(new_file->st->st_nlink))) > format->links_min_wid)
		format->links_min_wid = len;
	if ((len = ft_strlen(ft_lltoa(new_file->st->st_size))) > format->file_size)
		format->file_size = len;
	format->user_min_wid = owner_width(new_file->st, format);
	format->group_min_wid = group_width(new_file->st, format);
	if ((new_file->st->st_mode & S_IFMT) == S_IFLNK ||
		(new_file->st->st_mode & S_IFMT) == S_IFCHR ||
		(new_file->st->st_mode & S_IFMT) == S_IFBLK)
	{
		if ((len = ft_strlen(ft_itoa((new_file->st->st_rdev >> 24)))) > format->rdev_size)
			format->rdev_size = len;
		if ((len = ft_strlen(ft_itoa((new_file->st->st_rdev & 0xFFFFFF)))) > format->rdev2_size)
			format->rdev2_size = len;
	}
}

void	sort_nosort(t_fileinfo **files, t_fileinfo *new_file)
{
	t_fileinfo	*tmp;

	tmp = *files;
	if (!*files)
	{
		*files = new_file;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_file;
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

t_fileinfo	*sort_default(t_fileinfo **files, t_fileinfo *new_file)
{
	t_fileinfo	*head;
	t_fileinfo	*tmp;
	t_fileinfo	*prev;

	tmp = *files;
	head = *files;
	prev = NULL;
	if (!*files)
		return (new_file);
	while (tmp && ft_strcmp(tmp->filename, new_file->filename) < 0)
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

int 		compare_dates(struct stat *newst, struct stat *tmpst) //char *new_date, char	*tmp_date
{
	if (!newst || !tmpst)
		return (1);
	if (newst->st_mtime <= tmpst->st_mtime)
		return (1);
	return (0);
}

t_fileinfo	*sort_modtime(t_fileinfo **files, t_fileinfo *new_file)
{
	t_fileinfo		*tmp;
	t_fileinfo		*head;
	t_fileinfo		*prev;

	tmp = *files;
	head = *files;
	prev = NULL;
	if (!*files)
		return (new_file);
	while (tmp && compare_dates(new_file->st, tmp->st) > 0)
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

void	add_new_file(t_fileinfo **files, struct stat *st, t_all *all, t_to_ls *to_ls)
{
	t_fileinfo		*new_file;

	new_file = (t_fileinfo*)ft_memalloc(sizeof(t_fileinfo));
	new_file->st = st;
	new_file->filename = ft_strdup(all->filename); // this is now allocated memmory
	new_file->path = to_ls->path; // this is allocated memmory in to_ls
	new_file->next = NULL;
	field_widths(all->format, new_file);
	if (all->options->option_f)
		sort_nosort(files, new_file); // made
	else if (all->options->option_t)
		*files = sort_modtime(files, new_file); // made
	else if (all->options->option_r)
		*files = sort_reverse(files, new_file); // made
	else
		*files = sort_default(files, new_file); // made
}
// ^ finish this then test ^

char		*get_file_path(char *filefrom, char *d_name)
{
	char	*tmp;
	char	*ret;

	if (filefrom[ft_strlen(filefrom) - 1] == '/')
		ret = ft_strjoin(filefrom, d_name);
	else
	{
		tmp = ft_strjoin(filefrom, "/");
		ret = ft_strjoin(tmp, d_name);
		if (tmp)
			free(tmp);
	}
	return (ret);
}

t_fileinfo	*get_files_info(t_all *all, t_to_ls *to_ls)
{
	t_fileinfo		*files;
	DIR				*dir;
	struct dirent	*dirptr;
	struct stat		*st;

	files = NULL;
	dir = opendir(to_ls->name);
	while ((dirptr = readdir(dir)))
	{
		to_ls->path = get_file_path(to_ls->name, dirptr->d_name);
		st = (struct stat*)ft_memalloc(sizeof(struct stat));
		if (lstat(to_ls->path, st) == 0)
		{
			all->filename = dirptr->d_name;
			if (dirptr->d_name[0] == '.' && !all->options->option_a)
				free(st);
			else
				add_new_file(&files, st, all, to_ls);
		}
		else if (st)
			free(st);
	}
	closedir(dir);
	return (files);
}

void	add_new_dir(t_fileinfo *dir, t_to_ls **sub_dirs)
{
	t_to_ls			*new_dir;
	t_to_ls			*tmp;

	tmp = *sub_dirs;
	new_dir = (t_to_ls*)ft_memalloc(sizeof(t_to_ls));
	if (!dir)
		return ;
	new_dir->next = NULL;
	new_dir->name = ft_strdup(dir->path);
	new_dir->path = ft_strdup(dir->path);
	if (!tmp)
		*sub_dirs = new_dir;
	else
	{
		while(tmp->next)
			tmp = tmp->next;
		tmp->next = new_dir;
	}
}

t_to_ls *get_sub_dirs(t_fileinfo *files)
{
	t_to_ls			*sub_dirs;
	t_fileinfo		*tmp;

	tmp = files;
	sub_dirs = NULL;
	while (tmp)
	{
		if ((tmp->st->st_mode & S_IFMT) == S_IFDIR && tmp->filename[0] != '.')
			add_new_dir(tmp, &sub_dirs);
		tmp = tmp->next;
	}
	return (sub_dirs);
}

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
}

void	free_to_ls(t_to_ls **to_ls)
{
	t_to_ls 	*tmp;
	t_to_ls		*next;
	
	tmp = *to_ls;
	while (tmp)
	{
		next = tmp->next;
		free(tmp->name);
//		free(tmp->path);
		free(tmp);
		tmp = next;
	}
}

void	ft_ls(t_all	*all, t_to_ls *to_ls)
{
	t_to_ls			*tmp;
	t_to_ls			*sub_dirs;

	if (!all->format)
		all->format = (t_format*)ft_memalloc(sizeof(t_format));
	check_ls_paths(&to_ls);
	if (!(tmp = to_ls))
		return ;
	if (tmp->next || all->options->option_R)
		all->print_dir = 1;
	while (tmp)
	{
		if (tmp->ls_file)
			output_single_file(all, tmp);
		else
		{
			all->files = get_files_info(all, tmp);
			output_info(tmp, all);
		}
		ft_printf("\n");
		if (all->options->option_R)
		{
			sub_dirs = get_sub_dirs(all->files);
			if (sub_dirs)
			{
				free_files(&all->files);
				ft_ls(all, sub_dirs); // recursion.
			}
		}
		tmp = tmp->next;
		
	}
	free_to_ls(&to_ls);
}

int		main(int argc, char **argv)
{
	t_all		all;

	ft_bzero(&all, sizeof(t_all));
	all.options = (t_options*)ft_memalloc(sizeof(t_options));
	if (!all.options)
		return (0);
	all.to_ls = NULL;
	all.files = NULL;
	get_options(argc, argv, &all); // make this. checks for options
	if (!all.to_ls)
		parse_directory("./", (&all.to_ls));
	ft_ls(&all, all.to_ls);
	free(all.options);
	return (0);
}
