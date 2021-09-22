/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 02:25:21 by bmoulin           #+#    #+#             */
/*   Updated: 2021/08/26 02:25:24 by bmoulin          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	end_echo(t_list *exec, t_fd fd, t_shell *shell, t_bool newline)
{
	while (exec)
	{
		ft_putstr_fd(exec->content.word, fd);
		exec = exec->next;
		if (exec && exec->content.word)
			write(fd, " ", 1);
	}
	if (newline)
		write(fd, "\n", 1);
	shell->cmd_status = SUCESS;
	if (fd != 1)
		close(fd);
}

void	echo(t_list *lst, t_shell *shell, t_fd fd)
{
	t_bool	newline;
	t_list	*exec;

	exec = lst->lst_struct->exec;
	newline = TRUE;
	if (!exec->next)
	{
		ft_putstr_fd("\n", fd);
		return ;
	}
	if (exec)
		exec = exec->next;
	if (exec->content.word && exec->content.word[0] == '-'
		&& exec->content.word[1] && exec->content.word[1] == 'n'
		&& exec->content.word[2] == 0)
		newline = FALSE;
	if (newline == FALSE)
		exec = exec->next;
	end_echo(exec, fd, shell, newline);
}
