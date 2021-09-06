/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 11:53:28 by bmoulin           #+#    #+#             */
/*   Updated: 2021/08/26 11:53:31 by bmoulin          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_exec(t_exec *exec)
{
	exec->red_builtin[0] = &echo;
	exec->red_builtin[1] = &cd;
	exec->red_builtin[2] = &pwd;
	exec->red_builtin[3] = &export;
	exec->red_builtin[4] = &unset;
	exec->red_builtin[5] = &env;
	exec->red_builtin[6] = &exit_cmd;
	exec->red_builtin[7] = 0;
	exec->red_builtin[8] = &doo_execve;
}

void	exec_one_cmd(t_shell *shell)
{
	t_exec	*exec;

	exec = malloc(sizeof(t_exec));
	if (!exec)
		exit(1);
	init_exec(exec);
	shell->fd = get_fd(&shell->parse, shell);
	if (shell->fd != -1)
	{
		if (shell->read_fd > 0)
			dup2(shell->read_fd, 0);
		shell->builtin = is_it_a_builtin(shell->parse);
		if (shell->builtin == -1)
			exec->red_builtin[8](shell->parse, shell, shell->fd);
		else
			exec->red_builtin[shell->builtin](shell->parse, shell, shell->fd);
		if (shell->parse->next)
			shell->parse = shell->parse->next;
	}
	if (shell->fd > 1)
		close(shell->fd);
	if (shell->read_fd != -1)
		close(shell->read_fd);
	shell->read_fd = -1;
	free(exec);
}

void	start_pipe_cmds(t_shell *shell, t_pipe *pipes)
{
	pipes->pipes_set = 0;
	pipes->count = 0;
	pipes->exit_status = 0;
	pipes->nb_pipes = ft_lstsize(shell->parse) - 1;
	if (pipes->nb_pipes <= 0)
		return ;
	while (pipes->pipes_set != pipes->nb_pipes)
	{
		pipe(pipes->pipe_fd[pipes->pipes_set]);
		pipes->pipes_set++;
	}
	pipes->pipes_set = 0;
}

void	if_while_pipe(t_shell *shell, t_pipe *pipes)
{
	if (!pipes->cpid[pipes->count] && pipes->count == 0)
	{
		close(pipes->pipe_fd[0][0]);
		dup2(pipes->pipe_fd[0][1], 1);
		exec_one_cmd(shell);
		exit(0);
	}
	else if (!pipes->cpid[pipes->count] && pipes->count == pipes->nb_pipes)
	{
		close(pipes->pipe_fd[pipes->nb_pipes - 1][1]);
		dup2(pipes->pipe_fd[pipes->nb_pipes - 1][0], 0);
		exec_one_cmd(shell);
		exit(0);
	}
	else if (!pipes->cpid[pipes->count])
	{
		close(pipes->pipe_fd[pipes->count][0]);
		dup2(pipes->pipe_fd[pipes->count - 1][0], 0);
		close(pipes->pipe_fd[pipes->count - 1][1]);
		dup2(pipes->pipe_fd[pipes->count][1], 1);
		exec_one_cmd(shell);
		exit(0);
	}
}

void	do_pipe_cmd(t_shell *shell)
{
	t_pipe	*pipes;

	pipes = malloc(sizeof(t_pipe));
	if (!pipes)
		exit(0);
	start_pipe_cmds(shell, pipes);
	while (pipes->count != pipes->nb_pipes + 1)
	{
		pipes->cpid[pipes->count] = fork();
		if_while_pipe(shell, pipes);
		if (pipes->count < pipes->nb_pipes)
			close (pipes->pipe_fd[pipes->count][1]);
		shell->parse = shell->parse->next;
		pipes->count++;
	}
	pipes->count = 0;
	while (pipes->count != pipes->nb_pipes + 1)
	{
		do_waitpid(shell, pipes->cpid[pipes->count], &pipes->exit_status);
		pipes->count++;
	}
	free(pipes);
}
