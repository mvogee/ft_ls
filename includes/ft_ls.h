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

typedef struct			s_to_ls
{
	struct s_to_ls		*next;
	char				*path;
	char				*name;
	unsigned int		ls_file : 1;
}						t_to_ls;

typedef struct 			s_format
{
	int					serial_min_wid;
	int					links_min_wid;
	int					user_min_wid;
	int					group_min_wid;
	int					file_size;
	int					rdev_size;
	int					rdev2_size;
}						t_format;

typedef struct			s_fileinfo
{
	struct s_fileinfo	*next;
	struct stat			*st;
	char				*filename;
	char				*path;
}						t_fileinfo;

typedef struct			s_options
{
	unsigned int		option_up_r : 1;
	unsigned int		option_l : 1;
	unsigned int		option_a : 1;
	unsigned int		option_r : 1;
	unsigned int		option_t : 1;
	unsigned int		option_i : 1;
	unsigned int		option_up_f : 1;
	unsigned int		option_f : 1;
	unsigned int		option_n : 1;
}						t_options;

typedef struct 			s_all
{
	t_fileinfo			*files;
	t_to_ls				*to_ls;
	t_options			*options;
	t_format			*format;
	char				*filename;
	unsigned int		print_dir : 1;
}						t_all;

/*
** main.c
*/

void					throw_error(char *bad_info);
void					free_files(t_fileinfo **files);
void					free_to_ls(t_to_ls **to_ls);
void					ft_ls(t_all	*all, t_to_ls *to_ls);

/*
** parse_options.c
** 2 static
*/

void					get_options(int argc, char **argv, t_all *all);

/*
** parse_directory
** 2 static
*/

void					parse_directory(char *file,
							t_to_ls	**to_ls, unsigned int rev);

/*
** check_ls_paths.c
** 1 static
*/

void					check_ls_paths(t_to_ls **to_ls);

/*
** output.c
*/

void					extended_output_l(t_fileinfo *tmp, t_all *all);
void					follow_links(char *filepath);
void					output_size_or_sys(t_format *format, struct stat *st);
void					output_info(t_to_ls *directory, t_all *all);
void					output_single_file(t_all *all, t_to_ls *file);

/*
** output_two.c
*/

void					output_date(struct stat *st);
void					output_file_symbol(struct stat *st, char *filename);
void					print_blocksize(t_fileinfo *files, t_options *options);

/*
** output_three.c
*/

void					ouput_filetype(struct stat *st);
void					output_extended(char *path);
void					output_premissions(struct stat *st, char *path);
void					output_user_group_names(t_format *format,
												struct stat *st);

/*
** get_dirs_files.c
** 2 statics
*/

char					*get_file_path(char *filefrom, char *d_name);
t_fileinfo				*get_files_info(t_all *all, t_to_ls *to_ls);
t_to_ls					*get_sub_dirs(t_fileinfo *files);

/*
** field_widths.c
** 2 statics
*/

void					field_widths(t_format *format, t_fileinfo *new_file);

/*
** list_sort.c
** 1 static
*/

void					sort_nosort(t_fileinfo **files, t_fileinfo *new_file);
t_fileinfo				*sort_default(t_fileinfo **files,
										t_fileinfo *new_file);
t_fileinfo				*sort_modtime(t_fileinfo **files,
										t_fileinfo *new_file);


/*
** list_sort_rev.c
** 1 static
*/

t_fileinfo				*sort_reverse(t_fileinfo **files,
										t_fileinfo *new_file);
t_fileinfo				*sort_modtime_rev(t_fileinfo **files,
										t_fileinfo *new_file);

# endif
