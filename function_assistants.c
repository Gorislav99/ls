/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   function_assistants.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/24 19:30:40 by mgorczan          #+#    #+#             */
/*   Updated: 2019/05/24 19:30:42 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	shell_sort_word(char **str_temp, unsigned int lenght_file_name, t_flag flag)
{
	char	*temp;
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
			while ((j >= 0) && MODE_SORT_WORD)
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

void	exit_pr(t_file* file)
{
	free_file(file);
	perror("malloc");
	exit(1);
}


void	free_file(t_file *file_list)
{
	t_file	*temp;

	while(file_list != NULL)
	{
		temp = file_list->next;
		if (file_list->name != NULL)
			free(file_list->name);
		free(file_list);
		file_list = temp;
	}
}

void	free_dir(t_dir	*dir_list)
{
	t_dir *temp;

	while (dir_list != NULL)
	{
		temp = dir_list->next_dir;
		if (dir_list->first_file != NULL)
			free_file(dir_list->first_file);
		if (dir_list->name_dir != NULL)
		{
			// ft_printf("%s\n", dir_list->name_dir);
			free(dir_list->name_dir);
		}
		free(dir_list);
		dir_list = temp;
	}
}