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

void	cd_tild_inpath(char *path, t_shell *shell)
{
	char	*tmp;

	tmp = get_env_arg(shell->env, "HOME");
	(path)[0] = '/';
	path = ft_strjoin(tmp, path);
	if (!path)
		exit_free();
}

void	cd_exception(t_shell *shell, char *path, t_fd fd)
{
	char	*str;
	char	*tmp;

	str = ft_strdup("bash: cd: ");
	if (!str)
		exit_free();
	tmp = ft_strjoin(str, path);
	if (!tmp)
		exit_free();
	free(str);
	str = ft_strjoin(tmp, ": No such file or directory\n");
	if (!str)
		exit_free();
	free(tmp);
	if (fd == 1)
		write(2, str, str_len(str));
	else
		write(fd, str, str_len(str));
	free(str);
	shell->cmd_status = FAILED;
}

void	cd(t_list *lst, t_shell *shell, t_fd fd)
{
	DIR		*rep;
	t_cdd	cd;

	cd.exec = lst->lst_struct->exec;
	if (cd.exec->next == NULL)
	{
		shell->cmd_status = SUCESS;
		return ;
	}
	cd.savepwd = ret_pwd();
	cd.path = cd.exec->next->content.word;
	if (cd.path[0] == '~')
		cd_tild_inpath(cd.path, shell);
	rep = opendir(cd.path);
	chdir(cd.path);
	if (rep == NULL)
	{
		cd_exception(shell, cd.path, fd);
		free(cd.savepwd);
		return ;
	}
	closedir(rep);
	cd_move_oldpwd(cd.tmp, cd.path, shell, cd.savepwd);
	cd_move_pwd(cd.tmp, cd.path, shell, cd.savepwd);
}
