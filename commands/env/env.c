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

void	env(t_list *lst, t_shell *shell, t_fd fd)
{
	size_t	i;

	i = 0;
	lst = lst->next;
	while (shell->env[i])
	{
		ft_putstr_fd(shell->env[i], fd);
		ft_putstr_fd("\n", fd);
		i++;
	}
	shell->cmd_status = SUCESS;
}

void	env_all(t_list *lst, t_shell *shell, t_fd fd)
{
	size_t	i;

	i = 0;
	lst = lst->next;
	while (shell->env_all[i])
	{
		ft_putstr_fd(shell->env_all[i], fd);
		ft_putstr_fd("\n", fd);
		i++;
	}
	shell->cmd_status = SUCESS;
}
