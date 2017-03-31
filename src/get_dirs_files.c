/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_dirs_files.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/21/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/21/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static void	add_new_file(t_fileinfo **files, struct stat *st,
	t_all *all, t_to_ls *to_ls)
{
	t_fileinfo		*new_file;

	new_file = (t_fileinfo*)ft_memalloc(sizeof(t_fileinfo));
	new_file->st = st;
	new_file->filename = ft_strdup(all->filename); // this is now allocated memmory
	new_file->path = ft_strdup(to_ls->path); // this is allocated memmory in to_ls
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

static void	add_new_dir(t_fileinfo *dir, t_to_ls **sub_dirs)
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

t_to_ls 	*get_sub_dirs(t_fileinfo *files)
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
