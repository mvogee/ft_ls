/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/21/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/21/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include "ft_printf.h"
# include "libft.h"

typedef enum		e_errors
{
	INVALID_OPTION,
	NO_FILE,
	NO_RIGHTS
}					t_errors;

typedef struct		s_to_ls
{
	struct s_to_ls	*next;
	char			*name;
}					t_to_ls;

typedef struct		s_options
{
	unsigned int option_R : 1;
	unsigned int option_l : 1;
	unsigned int option_a : 1;
	unsigned int option_r : 1;
	unsigned int option_t : 1;
	unsigned int option_G : 1;
	t_to_ls				*to_ls; // directories given to be the subjects to look in
}					t_options;

/*
** main.c
*/

void	throw_error(int reason, char *bad_info);

/*
** parse_options.c
*/
// static t_to_ls	*add_new_sorted(t_to_ls **to_ls, t_to_ls *new)
void	parse_options(char *opt, t_options *options);
void	parse_directory(char *file, t_to_ls	**to_ls);
void	get_options(int argc, char **argv, t_options *options);

# endif
