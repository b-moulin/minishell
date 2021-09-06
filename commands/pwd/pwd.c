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

char	*ret_pwd(void)
{
	char	*info;

	info = malloc(sizeof(char) * 255);
	if (!info)
		return (NULL);
	if (getcwd(info, 255) == NULL)
		return (NULL);
	return (info);
}

void	pwd(t_list *lst, t_shell *shell, t_fd fd)
{
	char	info[255];

	lst = lst->lst_struct->exec->next;
	if (getcwd(info, 255) == NULL)
	{
		shell->cmd_status = 1;
	}
	ft_putstr_fd(info, fd);
	ft_putstr_fd("\n", fd);
	shell->cmd_status = SUCESS;
}
