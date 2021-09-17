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
	shell->cmd_status = shell->cmd_status;
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
