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

void	throw_error(int reason, char *bad_info)
{
	write(2, "ft_ls: ", 8);
	if (reason == INVALID_OPTION)
	{
		write(2, "illegal option -- ", 19);
		write(2, &bad_info[0], 1);
		write(2, "\nusage: ls [-aRrlt] [file ...]\n", 31);
	}
	// else if (reason == NO_FILE) // call ls if there are still files to ls
	// {
	// 	write(2, bad_info, ft_strlen(bad_info) + 1);
	// 	write(2, ": No such file or directory\n", 29);
	// }
	// else if (reason == NO_RIGHTS)
	// 	//print out ussage restriction message 

//	exit(EXIT_FAILURE); // we dont exit if there are more directories to check
}

// t_fileinfo	*get_files_info(t_options *options, char *filesfrom)
// {
// 	// make this after check_ls_paths
// }

t_to_ls	*bad_path(t_to_ls **to_ls, t_to_ls *bad_path)
{
	t_to_ls		*prev;
	t_to_ls		*head;

	head = *to_ls;
	if (!bad_path)
		return (NULL);
	if (ft_strequ(bad_path->name, head->name))
		head = head->next;
	else
	{
		prev = head;
		while (!ft_strequ(bad_path->name ,prev->next->name))
			prev = prev->next;
		prev->next = bad_path->next;
	}
	ft_memdel((void**)bad_path);
	return (head);
}

void	check_ls_paths(t_to_ls **to_ls)
{
	t_to_ls		*tmp;
	t_to_ls		*next;
	DIR			*d;

	tmp = *to_ls;
	while (tmp)
	{
		d = opendir(tmp->name);
		if (!d)
		{
			next = tmp->next;
			write(2, "ft_ls: ", 8);
			write(2, tmp->name, ft_strlen(tmp->name) + 1);
			write(2, ": No such file or directory\n", 29);
			*to_ls = bad_path(to_ls, tmp); // make this. removes tmp link from list
			tmp = next;
		}
		else
		{
			free(d);
			tmp = tmp->next;
		}
	}
	if (!(*to_ls))
		exit(EXIT_FAILURE);
}

void	ft_ls(t_options	*options, t_to_ls **to_ls)
{
	t_outinfo_gen	outinfo_gen;
	t_to_ls			*tmp;
	//t_fileinfo		*files;
	//t_to_ls			*sub_dirs;

	(void)options;
	ft_bzero(&outinfo_gen, (sizeof(t_outinfo_gen)));
	check_ls_paths(to_ls); //print an error and remove the path if non existent
	tmp = *to_ls;
	// make sure all the files in to_ls exist and can be reached from "."
	while (tmp)
	{ // get_files_info is where most of the magic will be happening
		//files = get_files_info(options, to_ls->name); // make this. should sort correctly as it goes. if premision is not allowed note that.
		//ouput_info(files); // make this. needs to ouput all the file information correclty
		// recurse here if we have -R
		// if (options->option_R) // for each to_ls there is a different set of files/ sub_dirs
		// {
		// 	sub_dirs = get_sub_dirs(files); // make this. if no directories found return NULL
		// 	if (sub_dirs)
		// 		ft_ls(option, sub_dirs); // recursion. make sure this is sage
		// }
		tmp = tmp->next;
	}
}

int		main(int argc, char **argv)
{
	t_options	options;

	ft_bzero(&options, sizeof(t_options));
	get_options(argc, argv, &options); // make this. checks for options
	if (!options.to_ls)
		parse_directory(".", (&options.to_ls));
	ft_ls(&options, &options.to_ls);

// printing stuff out
	ft_printf("options\nl: %d\nR: %d\na: %d\nr: %d\nt: %d\nG: %d\ni: %d\nF: %d\n", options.option_l, options.option_R, options.option_a, options.option_r, options.option_t, options.option_G, options.option_i, options.option_F);
// printing directories to list
	t_to_ls 	*tmp = options.to_ls;
	while (tmp)
	{
		ft_printf("to_ls: %s\n", tmp->name);
		tmp = tmp->next;
	}
	// once we have the options we need to get the files contained in the current directory
	return (0);
}
