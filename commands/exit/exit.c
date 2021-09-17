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

int	check_exit_int(t_shell *shell, char *arg, t_fd fd)
{
	int	i;

	i = 0;
	shell->cmd_status = shell->cmd_status + 0;
	while (arg && arg[i])
	{
		if ((arg[i] < '0' || arg[i] > '9') && arg[i] != '-')
		{
			ft_putstr_fd("exit\nbash: exit: ", fd);
			ft_putstr_fd(arg, fd);
			ft_putstr_fd(": numeric argument required\n", fd);
			return (-1);
		}
		i++;
	}
	return (1);
}

long long	check_exitcode(t_shell *shell, char *arg, t_fd fd)
{
	long long	ret_value;

	if (!arg)
		return (0);
	ret_value = ft_atoi(arg);
	shell->cmd_status = shell->cmd_status + 0;
	if (ret_value == -1 && ft_atoi(arg + 1) != 1)
	{
		ft_putstr_fd("exit\nbash: exit: ", fd);
		ft_putstr_fd(arg, fd);
		ft_putstr_fd(": numeric argument required\n", fd);
		return (-1);
	}
	ft_putstr_fd("exit\n", fd);
	if (ret_value > 255)
		return (ret_value % 256);
	return (ret_value);
}

void	end_exit_cmd(char **args, t_shell *shell, long long ret_value, t_fd fd)
{
	if (args && args[0] && args[1])
	{
		if (check_exit_int(shell, args[0], fd) == -1)
			exit(255);
		ft_putstr_fd("bash: exit: too many arguments\n", fd);
		shell->cmd_status = 1;
		return ;
	}
	if (args)
		ret_value = check_exit_int(shell, args[0], fd);
	else
		ret_value = check_exit_int(shell, NULL, fd);
	if (ret_value == -1)
		exit(255);
	if (args)
		ret_value = check_exitcode(shell, args[0], fd);
	else
		ret_value = check_exitcode(shell, NULL, fd);
	if (ret_value == -1)
		exit(255);
	exit(ret_value);
}

void	exit_part(t_ext	ext, t_list	*exec)
{
	ext.args = malloc(sizeof(char *) * 3);
	if (!ext.args)
		exit_free();
	ext.args[2] = 0;
	ext.args[0] = ft_strdup(exec->content.word);
	if (!ext.args[0])
		exit_free();
	ext.args[1] = ft_strdup(exec->next->content.word);
	if (!ext.args[1])
		exit_free();
}

void	exit_cmd(t_list *lst, t_shell *shell, t_fd fd)
{
	t_ext		ext;
	t_list		*exec;

	fd = 1;
	exec = lst->lst_struct->exec;
	if (!exec->next)
		exit(0);
	exec = exec->next;
	if (exec && exec->content.word && exec->next && exec->next->content.word)
	{
		exit_free();
	}
	else if (exec && exec->content.word)
	{
		ext.args = malloc(sizeof(char *) * 3);
		if (!ext.args)
			exit_free();
		ext.args[1] = 0;
		ext.args[0] = ft_strdup(exec->content.word);
		if (!ext.args[0])
			exit_free();
	}
	ext.ret_value = 0;
	end_exit_cmd(ext.args, shell, ext.ret_value, fd);
}
