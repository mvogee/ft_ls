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

// \/_______________ parsing argv ______________\/ 

void	parse_options(char *opt, t_options *options)
{
	int		i;

	i = 1;
	while (opt[i])
	{
		if (opt[i] == 'l')
			options->option_l = 1;
		else if (opt[i] == 'R')
			options->option_R = 1;
		else if (opt[i] == 'a')
			options->option_a = 1;
		else if (opt[i] == 'r')
			options->option_r = 1;
		else if (opt[i] == 't')
			options->option_t = 1;
		else
			throw_error(INVALID_OPTION, opt + i);
		i++;
	}
}

t_to_ls	*add_new_sorted(t_to_ls **to_ls, t_to_ls *new)
{
	t_to_ls		*head;
	t_to_ls		*tmp;
	t_to_ls		*prev;

	head = *to_ls;
	tmp = *to_ls;
	prev = tmp;
	while (tmp && ft_strcmp(new->name, tmp->name) > 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!tmp || !ft_strequ(tmp->name, prev->name))
	{
		new->next = tmp;
		prev->next = new;
	}
	else
	{
		new->next = tmp;
		head = new;
	}
	return (head);
}

void	parse_directory(char *file, t_to_ls	**to_ls) // need to sort this by ascii value
{
	t_to_ls		*new;

	new = (t_to_ls*)ft_memalloc(sizeof(t_to_ls));
	new->name = file;
	new->next = NULL;
	if (!(*to_ls))
		*to_ls = new;
	else
		*to_ls = add_new_sorted(to_ls, new);
}

void	get_options(int argc, char **argv, t_options *options)
{
	int			count;
	int			type;

	count = 1;
	type = 0;
	while (count < argc)
	{
		if (argv[count][0] == '-' && argv[count][1] != '\0' && type == 0)
			parse_options(argv[count], options);
		else
		{
			type = 1;
			parse_directory(argv[count], &options->to_ls);
		}
		count++;
	}
	ft_printf("options\nl: %d\nR: %d\na: %d\nr: %d\nt: %d\n", options->option_l, options->option_R, options->option_a, options->option_r, options->option_t);
}
//  ^_____________________parsing argv_______________________ ^

int		main(int argc, char **argv)
{
	t_options	options;

	ft_bzero(&options, sizeof(t_options));
	get_options(argc, argv, &options); // make this. checks for options
	if (!options.to_ls)
		parse_directory(".", (&options.to_ls));


// printing directories to list
	t_to_ls 	*tmp = options.to_ls;
	while (tmp)
	{
		ft_printf("to_ls: %s\n", tmp->name);
		tmp = tmp->next;
	}
	// once we have the options we need to get the files contained in the current directory
}
