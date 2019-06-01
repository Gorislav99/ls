/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dir.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/30 16:30:12 by mgorczan          #+#    #+#             */
/*   Updated: 2019/05/30 16:30:13 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static t_dir	*init_empty_dir()
{
	t_dir	*dir_list;

	if ((dir_list = (t_dir*)malloc(sizeof(t_dir))) == NULL)
		exit(1); //!!!
	dir_list->name_dir = NULL;
	dir_list->path = NULL;
	dir_list->dir = NULL;
	dir_list->max_spase[0] = 0;
	dir_list->max_spase[1] = 0;
	dir_list->max_spase[2] = 0;
	dir_list->max_tabs = 0;
	dir_list->error_num = 0;
	dir_list->lenght_file = 0;
	dir_list->first_file = NULL;
	dir_list->next_dir = NULL;
	return (dir_list);
}

static void		add_file(t_dir *dir, char *argv)
{
	int i;
	t_file *temp;

	i = 0;
	if (dir->first_file == NULL)
	{
		if ((dir->first_file = (t_file*)malloc(sizeof(t_file))) == NULL)
			exit(1);
		dir->first_file->name = argv;
		lstat(dir->first_file->name , &(dir->first_file->buff));
		dir->first_file->next = NULL;
	}
	else
	{
		temp = dir->first_file;
		while (temp->next != NULL)
			temp = temp->next;
		if ((temp->next = (t_file*)malloc(sizeof(t_file))) == NULL)
			exit(1);
		temp = temp->next;
		temp->name = argv;
		lstat(temp->name , &(temp->buff));
		temp->next = NULL;
	}
}

static t_dir	*add_dir(char *argv, t_flag flag)
{
	t_dir	*dir_list;

	dir_list = init_empty_dir();
	dir_list->dir = opendir(argv);
	if (!(dir_list->dir))
	{
		dir_list->name_dir = argv;
		dir_list->error_num = errno;
		return (dir_list);
	}
	dir_list->name_dir = argv;
	init_file(dir_list, flag);
	closedir(dir_list->dir);
	return (dir_list);
}

static t_dir	*insert_error_dir(t_dir *dir_list, t_dir *dir_insert)
{
	t_dir *temp;
	t_dir *temp2;

	

	if (dir_list->error_num == 0)
	{
		dir_insert->next_dir = dir_list;
		return (dir_insert);
	}
	if (strcmp(dir_list->name_dir, dir_insert->name_dir) > 0)
	{
		temp2 = dir_list;
		dir_list = dir_insert;
		dir_list->next_dir = temp2->next_dir;
		temp2->next_dir = NULL;
		dir_insert = temp2;
	}
	temp = dir_list;
	while (temp->next_dir != NULL && temp->next_dir->error_num != 0 && strcmp(temp->name_dir, dir_insert->name_dir) < 0)
	{
		temp = temp->next_dir;

	}
	if (temp->next_dir == NULL)
		return (dir_list);
	temp2 = temp->next_dir;
	temp->next_dir = dir_insert;
	dir_insert->next_dir = temp2;
	return (dir_list);
}

static t_dir	*add_dir_recursion(t_dir *dir, t_flag flag)
{
	t_file	*temp_file;
	t_dir	*temp_dir;
	// char	*temp_ch;

	if (dir->first_file == NULL)
		return (NULL);
	temp_file = dir->first_file;
	temp_dir = dir;
	while (temp_file != NULL)
	{
		if (S_ISDIR(temp_file->buff.st_mode) && strcmp(temp_file->name, "..") != 0 && strcmp(temp_file->name, ".") != 0)
		{
			temp_dir->path = ft_strjoin(dir->name_dir, "/");
			temp_dir->next_dir = add_dir(ft_strjoin(temp_dir->path, temp_file->name), flag);
			temp_dir = temp_dir->next_dir;
			temp_dir->next_dir = add_dir_recursion(temp_dir, flag);
			while (temp_dir->next_dir != NULL)
				temp_dir = temp_dir->next_dir;
			// free(temp_ch);
		}
		temp_file = temp_file->next;
	}
	return (dir->next_dir);
}

t_dir			*read_dir(int argc, char **argv, t_flag flag)
{
	int		i;
	t_dir	*dir_list;
	t_dir	*temp;
	t_dir	*temp2;
	struct stat	buff;

	dir_list = init_empty_dir();
	temp2 = dir_list;
	i = 0;
	temp = dir_list;
	if (i == argc)
	{
		temp->next_dir = add_dir(strdup("."), flag);
		temp->next_dir->path = strdup("./");
		temp2 = temp->next_dir;
		if (flag.R == 'R' && temp->next_dir)
			temp = add_dir_recursion(temp->next_dir, flag);
		return (dir_list);
	}
	while (i < argc)
	{
		if ((lstat(argv[i], &buff)) != -1 && (S_ISREG(buff.st_mode) || S_ISLNK(buff.st_mode)))
		{
			add_file(temp2, strdup(argv[i]));
			(temp2->lenght_file)++;
		}
		else
		{
			temp->next_dir = add_dir(strdup(argv[i]), flag);
			temp->next_dir->path = ft_strjoin(argv[i], "/");
			if (temp->next_dir->error_num != 0)
			{
				dir_list = insert_error_dir(dir_list, temp->next_dir);
				temp->next_dir = NULL;
			}
			if (flag.R == 'R' && temp->next_dir)
			{
				temp = temp->next_dir;
				temp->next_dir = add_dir_recursion(temp, flag);
				while (temp->next_dir != NULL)
					temp = temp->next_dir;
			}
			else if (temp->next_dir != NULL)
				temp = temp->next_dir;
		}
		i++;
	}
	if (flag.t != 't')
		sort_file(temp2, flag);
	else
		sort_file_tt(temp2, flag);

	return (dir_list);
}


/*
static t_dir	*add_dir(char *argv, t_flag flag)
{
	t_dir	*dir_list;

	dir_list = init_empty_dir();
	dir_list->dir = opendir(argv);
	if (!(dir_list->dir))
	{
		dir_list->name_dir = argv;
		if (errno == 13)
		{
			
		}
		else
			dir_list->error_num = errno;
		return (dir_list);
	}
	dir_list->name_dir = argv;
	init_file(dir_list, flag);
	closedir(dir_list->dir);
	return (dir_list);
}

static t_dir	*add_dir_recursion(t_dir *dir, t_flag flag)
{
	t_file	*temp_file;
	t_dir	*temp_dir;
	char	*temp_ch;

	if (dir->first_file == NULL)
		return (NULL);
	temp_file = dir->first_file;
	temp_dir = dir;
	while (temp_file != NULL)
	{
		if (S_ISDIR(temp_file->buff.st_mode) && strcmp(temp_file->name, "..") != 0 && strcmp(temp_file->name, ".") != 0)
		{
			temp_ch = ft_strjoin(dir->name_dir, "/");
			temp_dir->next_dir = add_dir(ft_strjoin(temp_ch, temp_file->name), flag);
			temp_dir = temp_dir->next_dir;
			temp_dir->next_dir = add_dir_recursion(temp_dir, flag);
			while (temp_dir->next_dir != NULL)
				temp_dir = temp_dir->next_dir;
			free(temp_ch);
		}
		temp_file = temp_file->next;
	}
	return (dir->next_dir);
}*/