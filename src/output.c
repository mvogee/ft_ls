/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvogee <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/22/03 16:43:26 by mvogee            #+#    #+#             */
/*   Updated: 2017/22/03 16:43:26 by mvogee           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	output_date(t_fileinfo *file)
{
	time_t	curtime;
	char	*filetime;
	char	*p2;
	int 	time_or_year;

	time_or_year = 0;
	curtime = time(NULL);
	filetime = ft_strsub(ctime(&file->st->st_mtime), 4, 6);
	if (!filetime)
		return ;
	if (file->st->st_mtime > curtime || file->st->st_mtime + SIX_MONTHS < curtime)
		p2 = ft_strsub(ctime(&file->st->st_mtime), 20, 4);
	else
		p2 = ft_strsub(ctime(&file->st->st_mtime), 11, 5);
	ft_printf("%s %5s ", filetime, p2); // this needs formattings
	if (filetime)
		free(filetime);
	if (p2)
		free(p2);
}

void	output_file_symbol(char rights[12], char *filename)
{
	if (rights[0] == 'd')
		write(1, "/", 1);
	else if (rights[3] == 'x' || rights[6] == 'x' || rights[9] == 'x')
		write(1, "*", 1);
	else if (rights[0] == 'l')
		write(1, "@", 1);
	else if (rights[0] == 's')
		write(1, "=", 1);
	else if (!filename || *filename == '\0')
		write(1, "%", 1);
	else if (rights[0] == 'p')
		write(1, "|", 1);
}

void	output_info(t_fileinfo *files, t_options *options, t_outinfo_gen *oi)
{
	t_fileinfo *tmp;

	tmp = files;
	// if (optons->option_G)
	// 	output_color(files, options, oi); // make this
	// else
	// { // move all this to other funciton
	while (tmp)
	{
		if (tmp->filename[0] == '.' && !options->option_a)
		{
			tmp = tmp->next;
			continue;
		}
		if (options->option_i)
			ft_printf("%*llu ", oi->serial_min_wid ,tmp->st->st_ino);
		if (options->option_l)
		{
			ft_printf("%s %*d ", tmp->rights, oi->links_min_wid, tmp->st->st_nlink);
			if (!options->option_n)
				ft_printf("%*s  %*s ",  oi->user_min_wid, tmp->owner_name, oi->group_min_wid, tmp->group_name);
			else
				ft_printf("%d  %d ",  tmp->st->st_uid, tmp->st->st_gid);
			ft_printf(" %*lld ", oi->file_size, tmp->st->st_size);
			output_date(tmp);
		}
		ft_printf("%s", tmp->filename);
		if (options->option_F)
			output_file_symbol(tmp->rights, tmp->filename);
		ft_printf("\n");
		tmp = tmp->next;
	}
//	}
}
