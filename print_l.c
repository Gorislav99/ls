/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_l.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgorczan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/01 14:14:09 by mgorczan          #+#    #+#             */
/*   Updated: 2019/06/01 14:14:10 by mgorczan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"
#include "sys/xattr.h"
#include "sys/acl.h"
#include "pwd.h"
#include "grp.h"


static char	determinate_type_file(mode_t st_mode)
{
	if ((st_mode & S_IFMT) == S_IFIFO)
		return ('p');
	else if ((st_mode & S_IFMT) == S_IFCHR)
		return ('c');
	else if ((st_mode & S_IFMT) == S_IFDIR)
		return ('d');
	else if ((st_mode & S_IFMT) == S_IFBLK)
		return ('b');
	else if ((st_mode & S_IFMT) == S_IFLNK)
		return ('l');
	else if ((st_mode & S_IFMT) == S_IFSOCK)
		return ('s');
	else if ((st_mode & S_IFMT) == S_IFWHT)
		return ('w');
	return ('-');
}

static char	get_x_attr(t_file *file, char *path)
{
    acl_t        acl;
    acl_entry_t    dummy;
    ssize_t        xattr;
    char        ret;
	char		*str;

	str = ft_strjoin(path, file->name);
    acl = acl_get_link_np(str, ACL_TYPE_EXTENDED);
    if (acl && acl_get_entry(acl, ACL_FIRST_ENTRY, &dummy) == -1)
    {
        acl_free(acl);
        acl = NULL;
    }
    xattr = listxattr(str, NULL, 0, XATTR_NOFOLLOW);
    if (xattr < 0)
        xattr = 0;
    if (xattr > 0)
        ret = '@';
    else if (acl != NULL)
        ret = '+';
    else
        ret = ' ';
	free(str);
    return (ret);
}

static char	*perm(t_file *file)
{
	char	*perm;
	mode_t	temp;
	int		i;

	if (file == NULL)
		return (NULL);
	if ((perm = (char*)malloc(sizeof(char) * 11)) == NULL)
		return (0);
	i = 0;
	temp = file->buff.st_mode;
	perm[0] = determinate_type_file(file->buff.st_mode);
	perm[1] = ((temp & S_IRUSR) == S_IRUSR) ? 'r' : '-';
	perm[2] = ((temp & S_IWUSR) == S_IWUSR) ? 'w' : '-';
	perm[3] = ((temp & S_IXUSR) == S_IXUSR) ? 'x' : '-';
	if (perm[3] != '-')
		perm[3] = ((temp & S_ISUID) == S_ISUID) ? 's' : perm[3];
	else
		perm[3] = ((temp & S_ISUID) == S_ISUID) ? 'S' : perm[3];
	perm[4] = ((temp & S_IRGRP) == S_IRGRP) ? 'r' : '-';
	perm[5] = ((temp & S_IWGRP) == S_IWGRP) ? 'w' : '-';
	perm[6] = ((temp & S_IXGRP) == S_IXGRP) ? 'x' : '-';
	if (perm[6] != '-')
		perm[6] = ((temp & S_ISGID) == S_ISGID) ? 's' : perm[6];
	else
		perm[6] = ((temp & S_ISGID) == S_ISGID) ? 'S' : perm[6];
	perm[7] = ((temp & S_IROTH) == S_IROTH) ? 'r' : '-';
	perm[8] = ((temp & S_IWOTH) == S_IWOTH) ? 'w' : '-';
	perm[9] = ((temp & S_IXOTH) == S_IXOTH) ? 'x' : '-';
	if (perm[9] != '-')
		perm[9] = ((temp & S_ISVTX) == S_ISVTX) ? 't' : perm[9];
	else
		perm[9] = ((temp & S_ISVTX) == S_ISVTX) ? 'T' : perm[9];
	
	perm[10] = '\0';
	return (perm);
}

static char	*mtime_file(t_file *file)
{
	char	*today;
	char	*mtime;
	int		temp_lenght;
	int		temp_lenght2;
	unsigned int		i;

	time_t	temp;

	temp = time(NULL);
	today = ctime(&temp);
	mtime = ctime(&(file->buff.st_mtimespec.tv_sec));
	temp_lenght = ft_strlen(today);
	temp_lenght2 = ft_strlen(mtime);
	while (today[temp_lenght] == mtime[temp_lenght2] && today[temp_lenght] != ' ')
	{
		temp_lenght--;
		temp_lenght2--;
	}
	i = 0;
	if (mtime[temp_lenght2] == ' ')
	{
		while (mtime[temp_lenght2] != ':')
			temp_lenght2--;
		while (mtime[i++] !=' ');
		return (ft_strsub(mtime, i, temp_lenght2 - i));
	}
	return (NULL);
}

void		print_file_l(t_dir *dir)
{
	char	*permission;
	t_file	*temp;
	struct passwd *p;
	struct group *p2;
	char	*str;
	temp = dir->first_file;
	while (temp != NULL)	
	{
		// getxattr()
		permission = perm(temp);
		// ft_printf("%s %c %i ", permission, get_x_attr(temp, dir->path), temp->buff.st_nlink);
		// p = getpwuid(temp->buff.st_uid);
		// ft_printf("%s ", p->pw_name);
		// p2 = getgrgid(temp->buff.st_gid);
		// ft_printf("%s ", p2->gr_name);
		// ft_printf("%i ", temp->buff.st_size);
		str = mtime_file(temp);
		ft_printf("%s ", str);

		ft_printf("%s\n", temp->name);
		temp = temp->next;
	}
}
