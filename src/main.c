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
	t_to_ls 	*tmp;
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
		if (all->options->option_R)
		{
			sub_dirs = get_sub_dirs(all->files);
			if (sub_dirs)
			{
				ft_printf("\n");
				free_files(&all->files);
				ft_ls(all, sub_dirs); // recursion.
			}
		}
		if ((tmp = tmp->next))
			ft_printf("\n");
	}
	if (all->files)
		free_files(&all->files);
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
	free(all.format);
	free(all.options);
	free_files(&all.files);
	return (0);
}
