/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gdanylov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/13 21:07:01 by gdanylov          #+#    #+#             */
/*   Updated: 2018/01/13 21:07:03 by gdanylov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "libft.h"
#include "get_next_line.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

t_list			*multifile_get_in_list(int fd, t_list **vika)
{
	t_list	*buf;
	t_list	*new;

	if (!vika)
	{
		*vika = ft_lstnew("", 1);
		(*vika)->content_size = fd;
		return (*vika);
	}
	buf = *vika;
	while (buf && (size_t)fd != buf->content_size)
		buf = buf->next;
	if (buf && buf->content_size == (size_t)fd)
		return (buf);
	new = ft_lstnew("", 1);
	new->content_size = fd;
	ft_lstadd(vika, new);
	return (*vika);
}

int				get_in_list(char *tmp, t_list *list, char **line)
{
	int		i;
	char	*tmp2;
	char	*g;

	i = 0;
	g = tmp;
	*line = ft_strsub(tmp, 0, ft_countstrclen(tmp, '\n'));
	if ((tmp = ft_strchr(tmp, '\n')) != NULL)
	{
		tmp++;
		tmp2 = list->content;
		list->content = ft_strdup(tmp);
		free(tmp2);
	}
	else
		ft_bzero(list->content, ft_strlen(list->content));
	free(g);
	return (1);
}

int				get_next_line(const int fd, char **line)
{
	static		t_list *vika;
	t_struct	s;

	s.buf = ft_strnew(BUFF_SIZE);
	s.tmp = ft_strnew(BUFF_SIZE);
	s.buf2 = multifile_get_in_list(fd, &vika);
	while ((s.ret = read(fd, s.buf, BUFF_SIZE)) > 0)
	{
		s.t = s.tmp;
		s.tmp = ft_strjoin(s.tmp, s.buf);
		free(s.t);
		ft_strclr(s.buf);
		if (ft_strchr(s.tmp, '\n'))
			break;
	}
	free(s.buf);
	s.d = s.tmp;
	s.tmp = ft_strjoin(s.buf2->content, s.tmp);
	free(s.d);
	if (ft_strlen(s.tmp) == 0 && s.ret == 0 && (vika == NULL || ft_strlen(s.buf2->content) == 0))
	{
		free(s.tmp);
		return (0);
	}
	if (s.ret == -1)
		return (-1);
	get_in_list(s.tmp, s.buf2, line);
	return (1);
}
