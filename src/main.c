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
		write(2, "\nusage: ls [-aRFfGilrt] [file ...]\n", 36);
	}
	// else if (reason == NO_FILE) // call ls if there are still files to ls
	// {
	// 	write(2, bad_info, ft_strlen(bad_info) + 1);
	// 	write(2, ": No such file or directory\n", 29);
	// }
	// else if (reason == NO_RIGHTS)
	// 	//print out ussage restriction message 

	exit(EXIT_FAILURE); // we dont exit if there are more directories to check
}
// \/___________ gets ____________ \/
char		*get_owner(struct stat *st, t_outinfo_gen *outinfo_gen)
{
	char			*owner;
	struct passwd	*pd;
	int				width;

	pd = getpwuid(st->st_uid);
	owner = pd->pw_name;
	width = ft_strlen(owner);
	if (width > outinfo_gen->user_min_wid)
		outinfo_gen->user_min_wid = width;
	return (owner);
}

char		*get_group(struct stat *st, t_outinfo_gen *outinfo_gen)
{
	char			*group;
	struct group	*grp;
	int				width;

	grp = getgrgid(st->st_gid);
	group = grp->gr_name;
	width = ft_strlen(group);
	if (width > outinfo_gen->group_min_wid)
		outinfo_gen->group_min_wid = width;
	return (group);
}

char		get_filetype(struct stat *st)
{
	int filetype;

	filetype = st->st_mode & S_IFMT;
	if (filetype == S_IFREG)
		return ('-');
	else if (filetype == S_IFBLK) // do we need all the extra ones besides - and d?
		return ('b');
	else if (filetype == S_IFCHR)
		return ('c');
	else if (filetype == S_IFDIR)
		return ('d');
	else if (filetype == S_IFIFO)
		return ('p');
	else if (filetype == S_IFLNK)
		return ('l');
	else if (filetype == S_IFSOCK)
		return ('s');
	else
		return ('-');
}

char		get_extended(t_outinfo_gen *outinfo_gen)
{
	acl_t		acl;
	acl_entry_t	acl_e;
	ssize_t		xattr;

	acl = NULL;
	xattr = 0;
	acl = acl_get_link_np(outinfo_gen->filepath, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &acl_e) == -1) {
		acl_free(acl);
		acl = NULL;
	}
	xattr = listxattr(outinfo_gen->filepath, NULL, 0, XATTR_NOFOLLOW);
	if (xattr > 0)
		return ('@');
	else if (acl != NULL)
		return ('+');
	else
		return (' ');
}

void		get_rights(struct stat *st, char rights[12], t_outinfo_gen *outinfo_gen)
{
	if (!rights)
		return ;
	rights[0] = get_filetype(st);
	rights[1] = (st->st_mode & S_IRUSR ? 'r' : '-');
	rights[2] = (st->st_mode & S_IWUSR ? 'w' : '-');
	rights[3] = (st->st_mode & S_IXUSR ? 'x' : '-');
	rights[4] = (st->st_mode & S_IRGRP ? 'r' : '-');
	rights[5] = (st->st_mode & S_IWGRP ? 'w' : '-');
	rights[6] = (st->st_mode & S_IXGRP ? 'x' : '-');
	rights[7] = (st->st_mode & S_IROTH ? 'r' : '-');
	rights[8] = (st->st_mode & S_IWOTH ? 'w' : '-');
	rights[9] = (st->st_mode & S_IXOTH ? 'x' : '-');
	rights[10] = get_extended(outinfo_gen);
	rights[11] = '\0';
}
//    ^ _______ gets __________ ^

void		field_widths(t_outinfo_gen *outinfo_gen, t_fileinfo *new_file)
{
	int		len;

	if ((len = ft_strlen(ft_lltoa((long long)new_file->st->st_ino))) > outinfo_gen->serial_min_wid)
		outinfo_gen->serial_min_wid = len;
	if ((len = ft_strlen(ft_itoa(new_file->st->st_nlink))) > outinfo_gen->links_min_wid)
		outinfo_gen->links_min_wid = len;
	if ((len = ft_strlen(ft_lltoa(new_file->st->st_size))) > outinfo_gen->file_size)
		outinfo_gen->file_size = len;
	// other field widths are set elsewhere
}

void	sort_nosort(t_fileinfo **files, t_fileinfo *new_file)
{
	t_fileinfo	*tmp;

	tmp = *files;
	if (!*files)
	{
		*files = new_file;
		return ;
	}
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_file;
}

t_fileinfo	*sort_reverse(t_fileinfo **files, t_fileinfo *new_file)
{
	t_fileinfo	*head;
	t_fileinfo	*tmp;
	t_fileinfo	*prev;

	tmp = *files;
	head = *files;
	prev = NULL;
	if (!*files)
		return (new_file);
	while (tmp && ft_strcmp(tmp->filename, new_file->filename) > 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!prev)
	{
		new_file->next = tmp;
		head = new_file;
	}
	else
	{
		prev->next = new_file;
		new_file->next = tmp;
	}
	return (head);
}

t_fileinfo	*sort_default(t_fileinfo **files, t_fileinfo *new_file)
{
	t_fileinfo	*head;
	t_fileinfo	*tmp;
	t_fileinfo	*prev;

	tmp = *files;
	head = *files;
	prev = NULL;
	if (!*files)
		return (new_file);
	while (tmp && ft_strcmp(tmp->filename, new_file->filename) < 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!prev)
	{
		new_file->next = tmp;
		head = new_file;
	}
	else
	{
		prev->next = new_file;
		new_file->next = tmp;
	}
	return (head);
}

int			conv_month(char *month)
{
	if (month[0] == 'J' && month[1] == 'a' && month[2] == 'n')
		return (1);
	else if (month[0] == 'F' && month[1] == 'e' && month[2] == 'b')
		return (2);
	else if (month[0] == 'M' && month[1] == 'a' && month[2] == 'r')
		return (3);
	else if (month[0] == 'A' && month[1] == 'p' && month[2] == 'r')
		return (4);
	else if (month[0] == 'M' && month[1] == 'a' && month[2] == 'y')
		return (5);
	else if (month[0] == 'J' && month[1] == 'u' && month[2] == 'n')
		return (6);
	else if (month[0] == 'J' && month[1] == 'u' && month[2] == 'l')
		return (7);
	else if (month[0] == 'A' && month[1] == 'u' && month[2] == 'g')
		return (8);
	else if (month[0] == 'S' && month[1] == 'e' && month[2] == 'p')
		return (9);
	else if (month[0] == 'O' && month[1] == 'c' && month[2] == 't')
		return (10);
	else if (month[0] == 'N' && month[1] == 'o' && month[2] == 'v')
		return (11);
	else if (month[0] == 'D' && month[1] == 'e' && month[2] == 'c')
		return (12);
	return (0);
}

int 		compare_dates(struct stat *newst, struct stat *tmpst) //char *new_date, char	*tmp_date
{
	if (!newst || !tmpst)
		return (1);
	if (newst->st_mtime <= tmpst->st_mtime)
		return (1);
	return (0);
}

t_fileinfo	*sort_modtime(t_fileinfo **files, t_fileinfo *new_file)
{
	t_fileinfo		*tmp;
	t_fileinfo		*head;
	t_fileinfo		*prev;

	tmp = *files;
	head = *files;
	prev = NULL;
	if (!*files)
		return (new_file);
	while (tmp && compare_dates(new_file->st, tmp->st) > 0)
	{
		prev = tmp;
		tmp = tmp->next;
	}
	if (!prev)
	{
		head = new_file;
		new_file->next = tmp;
	}
	else
	{
		prev->next = new_file;
		new_file->next = tmp;
	}
	return (head);
}

void	add_new_file(t_fileinfo **files, struct stat *st, t_options *options, t_outinfo_gen *outinfo_gen)
{
	t_fileinfo		*new_file;

	new_file = (t_fileinfo*)ft_memalloc(sizeof(t_fileinfo));
	new_file->st = st;
	new_file->filename = outinfo_gen->curfile;
	get_rights(st, new_file->rights, outinfo_gen); //sets new_file->rights
	if (!options->option_n)
	{
		new_file->owner_name = get_owner(st, outinfo_gen);
		new_file->group_name = get_group(st, outinfo_gen);
	}
	new_file->next = NULL;
	field_widths(outinfo_gen, new_file);// done set field widths
	if (options->option_f)
		sort_nosort(files, new_file); // made
	else if (options->option_t)
		*files = sort_modtime(files, new_file); // made
	else if (options->option_r)
		*files = sort_reverse(files, new_file); // made
	else
		*files = sort_default(files, new_file); // made
}
// ^ finish this then test ^

char		*get_corrected_path(char *filefrom, char *d_name)
{
	char	*tmp;
	char	*ret;

	if (filefrom[ft_strlen(filefrom)] == '/')
		ret = ft_strjoin(filefrom, d_name);
	else
	{
		tmp = ft_strjoin(filefrom, "/");
		ret = ft_strjoin(tmp, d_name);
		if (tmp)
			free(tmp);
	}
	return (ret);
}

t_fileinfo	*get_files_info(t_options *options, char *filesfrom, t_outinfo_gen *outinfo_gen)
{
	t_fileinfo		*files;
	DIR				*dir;
	struct dirent	*dirptr;
	struct stat		*st;
	char			*fixed_path;

	files = NULL;
	dir = opendir(filesfrom);
	while ((dirptr = readdir(dir)))
	{
		fixed_path = get_corrected_path(filesfrom ,dirptr->d_name);
		st = (struct stat*)ft_memalloc(sizeof(struct stat));
		if (lstat(dirptr->d_name, st) == 0 || lstat(fixed_path, st) == 0)
		{
			outinfo_gen->curfile = dirptr->d_name;
			outinfo_gen->filepath = fixed_path;
			add_new_file(&files, st, options, outinfo_gen);	
		}
		else
			free(st);
		if (fixed_path)
			free(fixed_path);
	}
	closedir(dir);
	return (files);
}

void	ft_ls(t_options	*options, t_to_ls **to_ls)
{
	t_outinfo_gen	outinfo_gen;
	t_to_ls			*tmp;
	t_fileinfo		*files;
	//t_to_ls			*sub_dirs;

	ft_bzero(&outinfo_gen, (sizeof(t_outinfo_gen)));
	check_ls_paths(to_ls); //print an error and remove the path if non existent
	tmp = *to_ls;
	// make sure all the files in to_ls exist and can be reached from "."
	while (tmp)
	{ // get_files_info is where most of the magic will be happening
		files = get_files_info(options, tmp->name, &outinfo_gen); // make this. should sort correctly as it goes. if premision is not allowed note that.
		ouput_info(files); // make this. needs to ouput all the file information correclty
		// recurse here if we have -R
		// if (options->option_R) // for each to_ls there is a different set of files/ sub_dirs
		// {
		// 	sub_dirs = get_sub_dirs(files); // make this. if no directories found return NULL
		// 	if (sub_dirs)
		// 		ft_ls(option, sub_dirs); // recursion. make sure this is sage
		// }
		tmp = tmp->next;
	}
	t_fileinfo	*tmpp = files;
	while (tmpp)
	{
		ft_printf("%s	%s	%s	%s\n", tmpp->filename, tmpp->rights, tmpp->owner_name, tmpp->group_name);
		tmpp = tmpp->next;
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
