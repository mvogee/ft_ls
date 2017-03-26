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

void	output_info(t_fileinfo *files, t_options *options, t_outinfo_gen *oi)
{
	t_fileinfo *tmp;

	tmp = files;
	if (optons->option_G)
		output_color(files, options, oi); // make this
	else
	{ // move all this to other funciton
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
		{ // need to take into account all the flags for special output as well.
		//	output_rights(tmp, options, oi); // make this
		//	output_links_or_sysnum(tmp, options, oi); // make this
		//	output_users(tmp, options, oi); // make this
		//	output_size(tmp, optoions, oi); // make this
		//	output_date(tmp, options, oi); // make this
			ft_printf("%s %*d ", tmp->rights, oi->links_min_wid, tmp->st->st_nlink);
			// here figure out if you need to print system number things instead of links
			ft_printf("%*s  %*s ",  oi->user_min_wid, tmp->owner_name, oi->group_min_wid, tmp->group_name);
			ft_printf(" %*lld ", oi->file_size, tmp->st->st_size);
			ft_printf("%s ", ctime(&files->st->st_mtime)); // this needs formattings
		}
		ft_printf("%s", tmp->filename);
		ft_printf("\n");
		tmp = tmp->next;
	}
}