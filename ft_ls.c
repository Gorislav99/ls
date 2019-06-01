/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/21 16:14:55 by mgorczan          #+#    #+#             */
/*   Updated: 2019/05/21 17:17:08 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


static void	init_flag(t_flag *flag)
{
	flag->a = 0;
	flag->l = 0;
	flag->R = 0;
	flag->r = 0;
	flag->t = 0;
}

static void	check_flag(t_flag *flag, char *argv)
{
	unsigned int i;

	i = 0;
	while (argv[i] != '\0')
	{
		if (argv[i] == 'l')
			flag->l = 'l';
		else if (argv[i] == 'R')
			flag->R = 'R';
		else if (argv[i] == 'a')
			flag->a = 'a';
		else if (argv[i] == 'r')
			flag->r = 'r';
		else if (argv[i] == 't')
			flag->t = 't';
		else
		{
			write(2,"ls: illegal option -- ", 22);
			write(2, &argv[i], ft_strlen(&argv[i]));
			write(2, "\nusage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n", 63);
			exit(1);
		}
		++i;
	}
}

int			main(int argc, char **argv)
{
	t_dir	*dir_list;
	t_dir	*temp;
	t_flag	flag;
	int		fl;
	unsigned int		count_dir;
	int		i;
	
	i = 1;
	count_dir = 0;
	init_flag(&flag);
	while (i < argc)
	{
		if (argv[i][0] != '-' || (argv[i][0] == '-' && argv[i][1] == '\0'))
			break;
		check_flag(&flag, &argv[i++][1]);
	}
	if (i < argc)
		shell_sort_word(&argv[i], (unsigned int)(argc - i), flag);
	dir_list = read_dir(argc - i, &argv[i], flag);
	print_dir_list(dir_list, flag, argc - i);
	free_dir(dir_list);
	return (0);
}















	// while (dir_list != NULL)
	// {
	// 	ft_printf("\ndir: %s\n\n", dir_list->name_dir);
	// 	if (dir_list->first_file != NULL)
	// 	{
	// 		while (dir_list->first_file != NULL)
	// 		{
	// 			ft_printf("file: %s\n", dir_list->first_file->name);
	// 			dir_list->first_file = dir_list->first_file->next;
	// 		}
	// 		ft_printf("\n");
	// 	}
	// 	dir_list = dir_list->next_dir;
	// }
