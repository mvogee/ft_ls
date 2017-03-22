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

typedef struct 		s_outinfo_general // to be used for all outputting for minimum width for respecive fields
{ // set each of these to the largest for each ouput set
	int 			serial_min_wid;
	int				links_min_wid;
	int				user_min_wid;
	int				group_min_wid;
}					t_outinfo_gen;

typedef struct		s_fileinfo
{
	struct s_fileinfo	*next;
	char				*filename;
	unsigned int		serial; // st_ino
	char				rights[12]; // st_mode // extract the string
	unsigned int		links; // st_nlink
	unsigned int		owner_name; //st_uid
	unsigned int		group_name; //st_gid
	unsigned int		devide_type; // st_rdev
	unsigned int		size_bytes; // st_size
	char				last_mod[13] // will be created from timespecs
}					t_fileinfo;

typedef struct		s_options
{
	unsigned int option_R : 1;
	unsigned int option_l : 1;
	unsigned int option_a : 1;
	unsigned int option_r : 1;
	unsigned int option_t : 1;
	unsigned int option_G : 1; // bonus
	unsigned int option_i : 1; // bonus
	unsigned int option_F : 1; // bonus
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
