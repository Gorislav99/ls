/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 13:50:19 by mgorczan          #+#    #+#             */
/*   Updated: 2019/05/30 13:50:21 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"


static void		insert_sort_time(t_file **temp_file, unsigned int lenght_file_name)
{
	t_file	*temp;
	unsigned int		inc;
	int		i;
	int		j;
 
	i = 0;
	while (i < lenght_file_name)
	{
		temp = temp_file[i];
		j = i - 1;
		while ((j >= 0) && (temp_file[j]->buff.st_mtime < temp->buff.st_mtime))
		{
			temp_file[j + 1] = temp_file[j];
			j--;
		}
		temp_file[j + 1] = temp;
		i++;
	}
}

static void		shell_sort_pointer(t_file **str_temp, unsigned int lenght_file_name, t_flag flag)
{
	t_file	*temp;
	int		inc;
	int		i;
	int		j;
 
	inc = lenght_file_name >> 1;
	while (inc > 0)
	{
		i = inc;
		while (i < lenght_file_name)
		{
			temp = str_temp[i];
			j = i - inc;
			while ((j >= 0) && MODE_SORT_POINTER)
			{
				str_temp[j + inc] = str_temp[j];
				j -= inc;
			}
			str_temp[j + inc] = temp;
			i++;
		}
		inc /= 2;
	}
}

void			sort_file_tt(t_dir *dir, t_flag flag)
{
	int		i;
	t_file	**temp_file;
	t_file	*temp;

	temp = dir->first_file;
	i = 0;
	if (temp == NULL)
		return ;
	temp = dir->first_file;
	if ((temp_file = (t_file**)malloc(sizeof(t_file*) * dir->lenght_file)) == NULL)
		exit(1);
	while (temp != NULL)
	{
		if (dir->max_tabs < ft_strlen(temp->name))
			dir->max_tabs = ft_strlen(temp->name);
		temp_file[i++] = temp;
		temp = temp->next;
	}
	insert_sort_time(temp_file, dir->lenght_file);
	if (flag.r == 'r')
	{
		temp = temp_file[dir->lenght_file - 1];
		dir->first_file = temp;
		i = dir->lenght_file - 2;
		while (i >=0)
		{
			temp->next = temp_file[i--];
			temp = temp->next;
		}
		temp->next = NULL;
	}
	else
	{
		temp = temp_file[0];
		dir->first_file = temp;
		i = 1;
		while (i < (dir->lenght_file))
		{
			temp->next = temp_file[i++];
			temp = temp->next;
		}
		temp->next = NULL;
	}
	dir->max_tabs = (dir->max_tabs / 8 + 1) * 8;
	free(temp_file);
}

void			sort_file(t_dir *dir, t_flag flag)
{
	t_file	**str_temp;
	t_file	*file_temp;
	int		i;

	i = 0;
	file_temp = dir->first_file;
	if (file_temp == NULL)
		return ;
	if ((str_temp = (t_file**)malloc(sizeof(t_file*) * dir->lenght_file)) == NULL)
		exit(1);
	while (i < dir->lenght_file)
	{
		if (dir->max_tabs < ft_strlen(file_temp->name))
			dir->max_tabs = ft_strlen(file_temp->name);
		str_temp[i] = file_temp;
		file_temp = file_temp->next;
		++i;
	}
	shell_sort_pointer(str_temp, dir->lenght_file, flag); // отсортировать указатели
	i = 0;
	dir->first_file = str_temp[0];
	file_temp = dir->first_file;
	i = 1;
	while (i < dir->lenght_file)
	{
		file_temp->next = str_temp[i++];
		file_temp = file_temp->next;
	}
	file_temp->next = NULL;
	dir->max_tabs = (dir->max_tabs / 8 + 1) * 8;
	free(str_temp);
}

void			init_file(t_dir *dir, t_flag flag)
{
	struct dirent	*entry;
	t_file			*file;
	t_file			*temp;
	char			*buff;
	char			*buff2;

	file = NULL;
	while ((entry = readdir(dir->dir)) != NULL)
	{
		if (entry->d_name[0] == '.' && flag.a != 'a')
			continue ;
		if (file == NULL)
		{
			if ((file = (t_file*)malloc(sizeof(t_file))) == NULL)
				EXIT;
			file->next = NULL;
			file->name = NULL;
			temp = file;
		}
		else if ((temp->next = (t_file*)malloc(sizeof(t_file))) == NULL)
			EXIT;
		(dir->lenght_file)++;
		temp = temp->next != NULL ? temp->next : temp;
		if ((temp->name = ft_strdup(entry->d_name)) == NULL)
			EXIT;
		buff2 = ft_strjoin(dir->name_dir, "/");
		buff = ft_strjoin(buff2, entry->d_name);
		lstat(buff, &(temp->buff));
		free(buff);
		free(buff2);
		temp->next = NULL;
	}
	dir->first_file = file;
	if (flag.t != 't')
		sort_file(dir, flag);
	else
		sort_file_tt(dir, flag);
}