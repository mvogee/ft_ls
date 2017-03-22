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
	write(2, "ls: ", 5);
	if (reason == INVALID_OPTION)
	{
		write(2, "illegal option -- ", 19);
		write(2, &bad_info[0], 1);
		write(2, "\nusage: ls [-aRrlt] [file ...]\n", 31);
	}
	else if (reason == NO_FILE) // call ls if there are still files to ls
	{
		write(2, bad_info, ft_strlen(bad_info) + 1);
		write(2, ": No such file or directory\n", 29);
	}
	// else if (reason == NO_RIGHTS)
	// 	//print out ussage restriction message 

	exit(EXIT_FAILURE);
}

void	ft_ls(t_options	*options)
{
	t_outinfo_gen	outinfo_gen;

	ft_bzero(&outinfo_gen, (sizeof(t_outinfo_gen)));
	// make sure all the files in to_ls exist and can be reached from "."
	// while we go through the files in options.to_ls
	// get all the files names and store them.
	// get all the information needed for the files. if premision is not allowed not that.
	// depending on if a sorting option is given sort the files in correct order.
	// go down the list of files displaying them with information based on what flags we have

	// if the -R option is present v
	// 		look for the first direcory in directory files
	//		recurse ft_ls 
}

int		main(int argc, char **argv)
{
	t_options	options;

	ft_bzero(&options, sizeof(t_options));
	get_options(argc, argv, &options); // make this. checks for options
	if (!options.to_ls)
		parse_directory(".", (&options.to_ls));
//	ft_ls(&options);

// printing stuff out
	ft_printf("options\nl: %d\nR: %d\na: %d\nr: %d\nt: %d\nG: %d\ni: %d\nF:", options.option_l, options.option_R, options.option_a, options.option_r, options.option_t, options.option_G, options.option_i, options.option_F);
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
