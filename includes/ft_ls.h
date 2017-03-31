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

typedef struct		s_to_ls
{
	struct s_to_ls	*next;
	char			*path;
	char			*name;
	unsigned int ls_file : 1;
}					t_to_ls;

typedef struct 		s_format // to be used for all outputting for minimum width for respecive fields
{ // set each of these to the largest for each ouput set
	int				serial_min_wid;
	int				links_min_wid;
	int				user_min_wid;
	int				group_min_wid;
	int				file_size;
	int				rdev_size;
	int				rdev2_size;
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
	unsigned int	print_dir : 1;
}					t_all;

/*
** main.c
*/

void				throw_error(char *bad_info);
void				free_files(t_fileinfo **files);
void				free_to_ls(t_to_ls **to_ls);
void				ft_ls(t_all	*all, t_to_ls *to_ls);

/*
** parse_options.c
** 1 static
*/

void				parse_options(char *opt, t_options **options);
void				parse_directory(char *file, t_to_ls	**to_ls);
void				get_options(int argc, char **argv, t_all *all);

/*
** check_ls_paths.c
** 1 static
*/

void				check_ls_paths(t_to_ls **to_ls);

/*
** output.c
*/

void	output_date(struct stat *st);
void	output_file_symbol(struct stat *st, char *filename);
void	print_blocksize(t_fileinfo *files, t_options *options);
void			ouput_filetype(struct stat *st);
void			output_extended(char *path);
void			output_premissions(struct stat *st, char *path);
void				output_user_group_names(t_format *format, struct stat *st);
void				follow_links(char *filepath);
void				output_size_or_sys(t_format *format, struct stat *st);
void				output_info(t_to_ls *directory, t_all *all);
void				output_single_file(t_all *all, t_to_ls *file);

/*
** get_dirs_files.c
** 2 statics
*/

char				*get_file_path(char *filefrom, char *d_name);
t_fileinfo			*get_files_info(t_all *all, t_to_ls *to_ls);
t_to_ls 			*get_sub_dirs(t_fileinfo *files);

/*
** field_widths.c
** 2 statics
*/

void				field_widths(t_format *format, t_fileinfo *new_file);

/*
** list_sort.c
** 1 static
*/

void				sort_nosort(t_fileinfo **files, t_fileinfo *new_file);
t_fileinfo			*sort_default(t_fileinfo **files, t_fileinfo *new_file);
t_fileinfo			*sort_reverse(t_fileinfo **files, t_fileinfo *new_file);
t_fileinfo			*sort_modtime(t_fileinfo **files, t_fileinfo *new_file);

# endif
