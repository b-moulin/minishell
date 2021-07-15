/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efarin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/18 11:00:20 by efarin            #+#    #+#             */
/*   Updated: 2021/02/18 11:00:22 by efarin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

typedef struct s_gnl
{
	int		i;
	int		l;
	int		r;
	char	c;
	char	*tmp;
}	t_gnl;

int	get_next_line(int fd, char **line);

#endif