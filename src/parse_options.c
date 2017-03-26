/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_optoins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/22/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/22/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void			parse_options(char *opt, t_options *options)
{
	int			i;

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
		else if (opt[i] == 'i') // bonus. cerial number
			options->option_i = 1;
		else if (opt[i] == 'G') // bonus. collor
			options->option_G = 1;
		else if (opt[i] == 'F') // bonus. file names followed by symbol
			options->option_F = 1;
		else if (opt[i] == 'f') // bonus. unsorted order
			{
				options->option_f = 1;
				options->option_a = 1;
			}
		else if (opt[i] == 'n') // bonus. display user and group as number
			options->option_n = 1;
		else
			throw_error(INVALID_OPTION, opt + i);
		i++;
	}
}

static t_to_ls	*add_new_sorted(t_to_ls **to_ls, t_to_ls *new)
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

void			parse_directory(char *file, t_to_ls	**to_ls)
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

void			get_options(int argc, char **argv, t_options *options)
{
	int			count;
	int			type;

	count = 1;
	type = 0;
	if (count < argc)
	{
		if (argv[1][0] == '-' && argv[1][1] == '-' && (argv[1][2] == '\0' ||
			argv[1][2] == ' '))
		{
			type = 1;
			count++;
		}
	}
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
}
