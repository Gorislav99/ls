/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 16:15:42 by mgorczan          #+#    #+#             */
/*   Updated: 2019/05/21 16:42:19 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

#include "dirent.h"
#include "libft/libft.h"
#include "libft/ft_printf/ft_printf.h"
#include "sys/types.h"
#include "sys/stat.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/ioctl.h"
#include "errno.h"
#include "pwd.h"
#include "time.h"

# define EXIT exit_pr(file)
# define MODE_SORT_WORD (flag.r != 'r' ? ft_strcmp(str_temp[j], temp) > 0 : ft_strcmp(str_temp[j], temp) < 0)
# define MODE_SORT_POINTER (flag.r != 'r' ? ft_strcmp(str_temp[j]->name, temp->name) > 0 : ft_strcmp(str_temp[j]->name, temp->name) < 0)

typedef struct		s_file
{
	char			*name;
	struct stat		buff;
	struct s_file	*next;
}					t_file;

typedef struct		s_dir 
{
	DIR 			*dir;
	char			*name_dir;
	char			*path;
	unsigned int	max_spase[3];
	unsigned int	max_tabs;
	errno_t			error_num;
	t_file			*first_file;
	unsigned int	lenght_file;
	struct s_dir	*next_dir;
}					t_dir;


typedef struct		s_flag
{
	char			l;
	char			R;
	char			a;
	char			r;
	char			t;
}					t_flag;

void			free_dir(t_dir	*dir_list);
void			exit_pr(t_file* file);
void			shell_sort_word(char **str_temp, unsigned int lenght_file_name, t_flag flag);
void			free_file(t_file *file);
void			print_file(t_file *file, unsigned int space, unsigned int k);
t_dir			*read_dir(int argc, char **argv, t_flag flag);
void			print_dir(t_dir *dir, t_flag flag);
void			init_file(t_dir *dir, t_flag flag);
void     		sort_file_tt(t_dir *dir, t_flag flag);
void    		sort_file(t_dir *dir, t_flag flag);
void			print_dir_list(t_dir *dir_list, t_flag flag, int argc);

void		print_file_l(t_dir *dir);


#endif
