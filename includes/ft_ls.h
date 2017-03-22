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

typedef struct		s_options
{
	unsigned int option_R : 1;
	unsigned int option_l : 1;
	unsigned int option_a : 1;
	unsigned int option_r : 1;
	unsigned int option_t : 1;
	char				**directories; // directories given to be the subjects to look in
}					t_options;

# endif