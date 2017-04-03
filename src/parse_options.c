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

static	void	parse_options_bonus(char *opt, t_options **options, int i)
{
	if (opt[i] == 'i')
		(*options)->option_i = 1;
	else if (opt[i] == 'F')
		(*options)->option_up_f = 1;
	else if (opt[i] == 'f')
	{
		(*options)->option_f = 1;
		(*options)->option_a = 1;
	}
	else if (opt[i] == 'n')
		(*options)->option_n = 1;
	else
		throw_error(opt + i);
}

static void		parse_options(char *opt, t_options **options)
{
	int			i;

	i = 1;
	while (opt[i])
	{
		if (opt[i] == 'l')
			(*options)->option_l = 1;
		else if (opt[i] == 'R')
			(*options)->option_up_r = 1;
		else if (opt[i] == 'a')
			(*options)->option_a = 1;
		else if (opt[i] == 'r')
			(*options)->option_r = 1;
		else if (opt[i] == 't')
			(*options)->option_t = 1;
		else
			parse_options_bonus(opt, options, i);
		i++;
	}
}


void			get_options(int argc, char **argv, t_all *all)
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
			parse_options(argv[count], &all->options);
		else
		{
			type = 1;
			parse_directory(argv[count], &all->to_ls, all->options->option_r);
		}
		count++;
	}
}
