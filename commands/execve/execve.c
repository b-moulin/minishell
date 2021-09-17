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

void	except_execve(t_exve exve, const char *command,
	char **argv, t_shell *shell)
{
	char	*tmp;
	char	*tmpp;
	int		i;
	int		fd;

	tmp = 0;
	i = 0;
	tmpp = 0;
	if (exve.exve == -1)
		exve.exve = execve(command, argv, shell->env);
	if (exve.exve == -1)
	{
		exve.envarg = ft_strdup(command);
		ft_putstr_fd("bash: ", 2);
		ft_putstr_fd(exve.envarg, 2);
		ft_putstr_fd(": command not found\n", 2);
		shell->cmd_status = FAILED;
		wait(NULL);
		tmpp = ft_itoa(shell->cmd_number);
		tmp = ft_strjoin("/tmp/", tmpp);
		free(tmpp);
		close(open(tmp, O_RDONLY | O_CREAT | O_TRUNC, S_IRWXU));
		free(tmp);
		exit(0);
		return ;
	}
	while (i != 100000000)
		i++;
	tmpp = ft_itoa(shell->cmd_number);
	tmp = ft_strjoin("/tmp/", tmpp);
	free(tmpp);
	fd = open(tmp, O_RDONLY);
	if (fd > 0)
	{
		close(fd);
		shell->cmd_status = 127;
	}
	free(tmp);
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
		i++;
		exec = exec->next;
	}
	do_execve(shell, cmd, args, fd);
	free_double_tab(args);
	free(cmd);
}
