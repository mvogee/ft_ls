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

	pd = getpwuid(st.st_uid);
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

	grp = getgrgid(st.st_gid);
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
	acl = acl_get_link_np(outinfo_gen->curfile, ACL_TYPE_EXTENDED);
	if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &acl_e) == -1) {
        acl_free(acl);
        acl = NULL;
    }
    xattr = listxattr(outinfo_gen->curfile, NULL, 0, XATTR_NOFOLLOW);
    if (xattr > 0)
    	return ('@');
    else if (acl != NULL)
    	return ('+');
    else
    	return (' ');
}

void		get_rights(struct stat *st, char *rights[12], t_outinfo_gen *outinfo_gen)
{
	if (!*rights)
		return ;
	*rights[0] = get_filetype(st);
	*rights[1] = (st->st_mode & S_IRUSR ? 'r' : '-');
	*rights[2] = (st->st_mode & S_IWUSR ? 'w' : '-');
	*rights[3] = (st->st_mode & S_IXUSR ? 'x' : '-');
	*rights[4] = (st->st_mode & S_IRGRP ? 'r' : '-');
	*rights[5] = (st->st_mode & S_IWGRP ? 'w' : '-');
	*rights[6] = (st->st_mode & S_IXGRP ? 'x' : '-');
	*rights[7] = (st->st_mode & S_IROTH ? 'r' : '-');
	*rights[8] = (st->st_mode & S_IWOTH ? 'w' : '-');
	*rights[9] = (st->st_mode & S_IXOTH ? 'x' : '-');
	*rights[10] = get_extended(outinfo_gen);
	*rights[11] = '\0';
}
//    ^ _______ gets __________ ^

t_fileinfo	*add_new_file(t_fileinfo **files, struct stat *st, t_options *options, t_outinfo_gen *outinfo_gen)
{
	t_fileinfo		*new_file;

	new_file = (t_fileinfo*)ft_memmalloc(sizeof(t_fileinfo));
	new_file->st = st;
	new_file->filename = outinfo_gen->curfile;
	get_rights(st, &new_file->rights, outinfo_gen); sets new_file->rights
	if (!optoins->option_n)
	{
		new_file->owndr_name = get_owner(st, outinfo_gen);
		new_file->group_name = get_group(st, outinfo_gen);
	}
	new_file->next = NULL;
	field_widths(outinfo_gen, st, new_file); // make this. set field widths
	if (options->option_f)
		sort_nosort(files, new_file); // make this
	else if (options->option_t)
		sort_modtime(files, new_file); // make this
	else if (options->option_r)
		sort_reverse(files, new_file); // make this
	else
		sort_default(files, new_file); // make this
}
// ^ finish this then test ^
t_fileinfo	*get_files_info(t_options *options, char *filesfrom, t_outinfo_gen *outinfo_gen)
{
	t_fileinfo		*files;
	DIR				*dir;
	struct dirent	*dirptr;
	struct stat		*st;

	fileinfo = NULL;
	dir = opendir(filesfrom);
	while ((dirptr = readdir(dir))
	{
		st = (struct stat*)ft_memalloc(sizeof(struct stat));
		if (lstat(dirptr->d_name ,st) == 0)
		{
			outinfo_gen->curfile = filesfrom;
			add_new_file(&files, st, options, outinfo_gen);// make this // options for once we have the info for inserting in our list sorted		
		}
		else
			free(st);
	}
	closedir(filesfrom);
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
		files = get_files_info(options, to_ls->name, &outinfo_gen); // make this. should sort correctly as it goes. if premision is not allowed note that.
		//ouput_info(files); // make this. needs to ouput all the file information correclty
		// recurse here if we have -R
		// if (options->option_R) // for each to_ls there is a different set of files/ sub_dirs
		// {
		// 	sub_dirs = get_sub_dirs(files); // make this. if no directories found return NULL
		// 	if (sub_dirs)
		// 		ft_ls(option, sub_dirs); // recursion. make sure this is sage
		// }
		tmp = tmp->next;
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
