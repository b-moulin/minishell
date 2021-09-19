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
#include <string.h>
#include <assert.h>
#include <errno.h>

void	except_execve(t_exve exve, const char *command,
	char **argv, t_shell *shell)
{
	t_expt	expt;

	expt.tmp = 0;
	expt.i = 0;
	expt.tmpp = 0;
	if (exve.exve == -1)
	{
		exve.exve = execve(command, argv, shell->env);
		exve.envarg = ft_strdup(command);
		expt_exve(expt, shell, exve);
	}
	while (expt.i != 100000000)
		expt.i++;
	expt.tmpp = ft_itoa(shell->cmd_number);
	expt.tmp = ft_strjoin("/tmp/", expt.tmpp);
	free(expt.tmpp);
	expt.fd = open(expt.tmp, O_RDONLY);
	if (expt.fd > 0)
	{
		close(expt.fd);
		shell->cmd_status = 127;
	}
	free(expt.tmp);
}

void	pid_is_zero(t_shell *shell, const char *cmd, char **argv, t_exve exve)
{
	char	*tmp;

	tmp = 0;
	while (exve.path[exve.i])
	{
		tmp = ft_strjoin(exve.path[exve.i++], "/");
		if (!tmp)
			exit_free();
		exve.all_path = ft_strjoin(tmp, cmd);
		if (!exve.all_path)
			exit_free();
		free(tmp);
		exve.exve = execve(exve.all_path, argv, shell->env);
		free(exve.all_path);
	}
}

void	do_execve(t_shell *shell, const char *command, char **argv, int fd)
{
	t_exve	exve;

	exve.i = 0;
	exve.exit_status = 0;
	exve.pid = fork();
	exve.exve = exve.pid;
	if (exve.exve == 0)
	{
		exve.exve = execve(command, argv, shell->env);
		exve.envarg = get_env_arg(shell->env, "PATH");
		if (!exve.envarg)
			no_path(shell, command, exve);
		exve.path = ft_split(exve.envarg, ':');
		if (!exve.path)
			exit_free();
		if (fd != 1)
			dup2(fd, 1);
		pid_is_zero(shell, command, argv, exve);
	}
	do_waitpid(shell, exve.pid, &exve.exit_status);
	if (exve.exve == 0)
		exit(0);
	except_execve(exve, command, argv, shell);
	if (fd > 1)
		close(fd);
}

void	distrib_execve(t_shell *shell, char *cmd, char **args, int fd)
{
	do_execve(shell, cmd, args, fd);
	free_double_tab(args);
	free(cmd);
}

void	doo_execve(t_list *lst, t_shell *shell, t_fd fd)
{
	int		i;
	char	*cmd;
	char	**args;
	t_list	*exec;

	exec = lst->lst_struct->exec;
	if (!exec)
		return ;
	cmd = ft_strdup(exec->content.word);
	i = ft_lstsize(exec);
	args = 0;
	args = malloc(sizeof(char *) * (i + 1));
	if (!args)
		return ;
	args[i] = 0;
	i = 0;
	while (exec)
	{
		args[i] = ft_strdup(exec->content.word);
		if (!args[i])
			exit_free();
		i++;
		exec = exec->next;
	}
	distrib_execve(shell, cmd, args, fd);
}
