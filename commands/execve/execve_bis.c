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

void	expt_exve(t_expt expt, t_shell *shell, t_exve exve)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd(exve.envarg, 2);
	ft_putstr_fd(": command not found\n", 2);
	shell->cmd_status = FAILED;
	wait(NULL);
	expt.tmpp = ft_itoa(shell->cmd_number);
	expt.tmp = ft_strjoin("/tmp/", expt.tmpp);
	free(expt.tmpp);
	close(open(expt.tmp, O_RDONLY | O_CREAT | O_TRUNC, S_IRWXU));
	free(expt.tmp);
	exit(0);
}

void	no_path(t_shell *shell, const char *command, t_exve	exve)
{
	ft_putstr_fd("bash: ", 2);
	ft_putstr_fd((char *)command, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	exve.all_path = ft_itoa(shell->cmd_number);
	exve.envarg = ft_strjoin("/tmp/", exve.all_path);
	free(exve.all_path);
	close(open(exve.envarg, O_RDONLY | O_CREAT | O_TRUNC, S_IRWXU));
	free(exve.envarg);
	exit(0);
}

t_shell	*init_main(char **envp)
{
	t_shell	*shell;

	shell = malloc(sizeof(t_shell));
	if (!shell)
		exit(1);
	init_env(envp, shell);
	shell->parse = NULL;
	shell->builtin = 0;
	shell->fd = 1;
	shell->zero_fd = dup(0);
	shell->un_fd = dup(1);
	shell->cmd_status = 0;
	shell->cmd_number = 0;
	ret_shell_pointeur(shell);
	return (shell);
}
