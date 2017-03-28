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
# include <sys/types.h>
# include <sys/dir.h>
# include <dirent.h>
# include <sys/stat.h>
# include <time.h>
# include <sys/xattr.h>
# include <pwd.h>
# include <sys/acl.h>
# include <grp.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>

# define SIX_MONTHS 15552000

typedef enum		e_errors
{
	INVALID_OPTION,
	NO_FILE,
	NO_RIGHTS
}					t_errors;

typedef struct		s_to_ls
{
	struct s_to_ls	*next;
	char			*path;
	char			*name;
}					t_to_ls;

typedef struct 		s_format // to be used for all outputting for minimum width for respecive fields
{ // set each of these to the largest for each ouput set
	int				serial_min_wid;
	int				links_min_wid;
	int				user_min_wid;
	int				group_min_wid;
	int				file_size;
}					t_format;

typedef struct		s_fileinfo
{
	struct s_fileinfo	*next;
	struct stat			*st;
	char				*filename;
	char				*path;
}					t_fileinfo;

typedef struct		s_options
{
	unsigned int option_R : 1; // recrusive listing
	unsigned int option_l : 1; // extended info
	unsigned int option_a : 1; // include hidden files
	unsigned int option_r : 1; // reverse order
	unsigned int option_t : 1; // mod time sort
	unsigned int option_G : 1; // bonus color
	unsigned int option_i : 1; // bonus serial number
	unsigned int option_F : 1; // bonus file names followed by symbol
	unsigned int option_f : 1; // bonus unsorted order
	unsigned int option_n : 1; // bonus display user and group as number
}					t_options;

typedef struct 		s_all
{
	t_fileinfo		*files;
	t_to_ls			*to_ls;
	t_options		*options;
	t_format		*format;
	char			*filename;
}					t_all;

/*
** main.c
*/

void	throw_error(int reason, char *bad_info);

/*
** parse_options.c
*/
// static t_to_ls	*add_new_sorted(t_to_ls **to_ls, t_to_ls *new)
void	parse_options(char *opt, t_options **options);
void	parse_directory(char *file, t_to_ls	**to_ls);
void	get_options(int argc, char **argv, t_all *all);

/*
** check_ls_paths.c
*/

// static t_to_ls	*bad_path(t_to_ls **to_ls, t_to_ls *bad_path)
void			check_ls_paths(t_to_ls **to_ls);

/*
** output.c
*/

//void	output_info(t_fileinfo *files, t_options *options, t_outinfo_gen *oi);

# endif
