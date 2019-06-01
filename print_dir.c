/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_dir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 16:29:22 by mgorczan          #+#    #+#             */
/*   Updated: 2019/05/30 16:29:25 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


void		print_file(t_file *file, unsigned int space, unsigned int k)
{
	unsigned int	i;
	int	temp;
	unsigned int	k_temp;
	t_file			*temp_file;

	k_temp = k;
	while (k-- != 0)
	{
		temp_file = file;
		while (temp_file != NULL)
		{
			i = 0;
			ft_printf("%s", temp_file->name);
			temp = space - ft_strlen(temp_file->name);
			while (temp_file != NULL && i++ < k_temp)
				temp_file = temp_file->next;
			if (temp_file != NULL)
				while (temp-- > 0)
					ft_printf(" ");
		}
		ft_printf("\n");
		file = file->next;
	}
}




void		print_dir(t_dir *dir, t_flag flag)
{
	struct winsize	w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	if (dir->first_file == NULL)
		return ;
	
	if (dir->max_tabs > w.ws_col)
		print_file(dir->first_file, dir->lenght_file, dir->lenght_file);
	else if ((dir->max_tabs * dir->lenght_file) < w.ws_col)
		print_file(dir->first_file, dir->max_tabs, 1);
	else
		print_file(dir->first_file, dir->max_tabs, dir->lenght_file / (w.ws_col / dir->max_tabs) + (dir->lenght_file % (w.ws_col / dir->max_tabs) == 0 ? 0 : 1));
}

static int flag_name(t_dir *temp)
{
	if (temp->name_dir == NULL)
	{
		if (temp->next_dir == NULL)
			return (0);
		if (temp->first_file == NULL && temp->next_dir != NULL && temp->next_dir->next_dir == NULL)
			return (0);
		return (1);
	}
	while (temp->next_dir != NULL && temp->next_dir->name_dir != NULL)
		temp = temp->next_dir;
	if (temp->next_dir != NULL && temp->next_dir->next_dir == NULL)
		return (0);
	return (1);
}

static void	print_error(t_dir *dir, int fl)
{
	int		i;
	char	*ch;

	if (dir->error_num != 13)
	{
		write(2, "ls: ", 4);
		write(2, dir->name_dir, ft_strlen(dir->name_dir));
		write(2, ": ", 2);
		write(2,  strerror(dir->error_num), ft_strlen( strerror(dir->error_num))); //permision danied in dir
		write(2, "\n", 1);
	}
	else
	{
		if (dir->name_dir != NULL && fl && strcmp(".", dir->name_dir) != 0)
			ft_printf("%s:\n", dir->name_dir);
		i = ft_strlen(dir->name_dir) - 1;
		while (i > 0 && dir->name_dir[i] != '/')
			i--;
		if (dir->name_dir[i] == '/')
			i++;
		write(2, "ls: ", 4);
		write(2, &(dir->name_dir[i]), ft_strlen(&(dir->name_dir[i])));
		write(2, ": ", 2);		
		write(2,  strerror(dir->error_num), ft_strlen( strerror(dir->error_num))); //permision danied in dir
		write(2, "\n", 1);
		if (dir->next_dir != NULL)
			ft_printf("\n");
	}
	
}

void		print_dir_list(t_dir *dir_list, t_flag flag, int argc)
{
	int	fl;

	fl = flag_name(dir_list);
	print_file_l(dir_list);
	while (dir_list != NULL)
	{	
		if (dir_list->first_file != NULL && dir_list->error_num == 0)
		{
			if (dir_list->name_dir != NULL && strcmp(".", dir_list->name_dir) == 0 && fl && argc != 1)
				ft_printf("%s:\n", dir_list->name_dir);
			if (dir_list->name_dir != NULL && fl && strcmp(".", dir_list->name_dir) != 0)
				ft_printf("%s:\n", dir_list->name_dir);
			if (dir_list->name_dir != NULL)
				dir_list->dir = opendir(dir_list->name_dir);
			if (flag.l != 'l')
				print_dir(dir_list, flag);
			else
				print_file_l(dir_list);
			if (dir_list->next_dir != NULL)
				ft_printf("\n");
			if (dir_list->name_dir != NULL && dir_list->dir != NULL)
				closedir(dir_list->dir);
		}
		else if (dir_list->first_file == NULL && dir_list->error_num == 0)
		{
			if (dir_list->name_dir != NULL && fl)
				ft_printf("%s:\n", dir_list->name_dir);
			if (dir_list->name_dir != NULL && dir_list->next_dir != NULL)
				ft_printf("\n");
		}
		else if (dir_list->name_dir != NULL && dir_list->error_num != 0)
			print_error(dir_list, fl);
		dir_list = dir_list->next_dir;
	}
}