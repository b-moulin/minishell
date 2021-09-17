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

void	do_waitpid(t_shell *shell, pid_t pid, int *i)
{
	int	exit_status1;

	waitpid(pid, i, 0);
	exit_status1 = WEXITSTATUS(*i);
	shell->cmd_status = exit_status1;
}

void	exve_is_neg(t_exve exve,
	t_shell *shell, t_exept_exve expt)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(exve.envarg, 2);
	ft_putstr_fd(": command not found\n", 2);
	shell->cmd_status = FAILED;
	wait(NULL);
	expt.tmpp = ft_itoa(shell->cmd_number);
	if (!expt.tmpp)
		exit_free();
	expt.tmp = ft_strjoin("/tmp/", expt.tmpp);
	if (!expt.tmp)
		exit_free();
	free(expt.tmpp);
	close(open(expt.tmp, O_RDONLY | O_CREAT | O_TRUNC, S_IRWXU));
	free(expt.tmp);
	exit(0);
}

void	except_execve(t_exve exve, const char *command,
	char **argv, t_shell *shell)
{
	t_exept_exve	expt;

	expt.tmp = 0;
	expt.i = 0;
	expt.tmpp = 0;
	if (exve.exve == -1)
	{
		exve.exve = execve(command, argv, shell->env);
		exve.envarg = ft_strdup(command);
		if (!exve.envarg)
			exit_free();
		exve_is_neg(exve, shell, expt);
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

void	do_execve(t_shell *shell, const char *command, char **argv, int fd)
{
	t_exve	exve;
	char	*tmp;

	exve.i = 0;
	exve.exit_status = 0;
	exve.pid = fork();
	exve.exve = exve.pid;
	if (exve.exve == 0)
	{
		exve.envarg = get_env_arg(shell->env, "PATH");
		exve.path = ft_split(exve.envarg, ':');
		if (fd != 1)
			dup2(fd, 1);
		while (exve.path[exve.i])
		{
			tmp = ft_strjoin(exve.path[exve.i++], "/");
			exve.all_path = ft_strjoin(tmp, command);
			free(tmp);
			exve.exve = execve(exve.all_path, argv, shell->env);
			free(exve.all_path);

		}
	}
	except_execve(exve, command, argv, shell);
	if (fd > 1)
		close(fd);
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
	do_execve(shell, cmd, args, fd);
	free_double_tab(args);
	free(cmd);
}
