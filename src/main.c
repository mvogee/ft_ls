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
	write(2, "ft_ls: illegal option -- ", 26);
	write(2, &bad_info[0], 1);
	write(2, "\nusage: ls [-aRFfilrt] [file ...]\n", 35);
	exit(EXIT_FAILURE);
}

void	recursive(t_all *all)
{
	t_to_ls			*sub_dirs;

	sub_dirs = get_sub_dirs(all->files);
	if (sub_dirs)
	{
		ft_printf("\n");
		free_files(&all->files);
		ft_ls(all, sub_dirs); // recursion.
	}
}

void	ft_ls(t_all	*all, t_to_ls *to_ls)
{
	t_to_ls			*tmp;

	tmp = to_ls;
	if (!all->format)
		all->format = (t_format*)ft_memalloc(sizeof(t_format));
	check_ls_paths(&to_ls);
	if (tmp && (tmp->next || all->options->option_up_r))
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
		if (all->options->option_up_r)
			recursive(all);
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
		parse_directory("./", (&all.to_ls), 0);
	ft_ls(&all, all.to_ls);
	free(all.format);
	free(all.options);
	free_files(&all.files);
	return (0);
}
