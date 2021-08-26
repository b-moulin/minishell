/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   minishell.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aviscogl <aviscogl@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2021/08/26 02:25:21 by bmoulin      #+#   ##    ##    #+#       */
/*   Updated: 2021/08/26 05:15:27 by aviscogl    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */
#include "../minishell.h"

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	if (!(!s || !fd))
	{
		while (s[i])
		{
			write(fd, &s[i], 1);
			i++;
		}
	}
}

int	right(char	*name)
{
	int		fd;

	close(open(name, O_RDONLY | O_CREAT | O_TRUNC, S_IRWXU));
	fd = open(name, O_TRUNC | O_RDWR);
	return (fd);
}

int	double_right(char	*name)
{
	int		fd;

	close(open(name, O_WRONLY | O_CREAT | O_APPEND | O_CREAT, S_IRWXU));
	fd = open(name, O_WRONLY | O_CREAT | O_APPEND);
	return (fd);
}

int	left(char	*name)
{
	int		fd;

	fd = open(name, O_CREAT, S_IRUSR);
	if (fd < 0)
	{
		printf("error !\n");
		return (1);
	}
	return (fd);
}
