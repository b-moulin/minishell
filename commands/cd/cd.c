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
}

void	cd_exception(t_shell *shell, char *path, t_fd fd)
{
	char	*str;
	char	*tmp;

	str = ft_strdup("bash: cd: ");
	tmp = ft_strjoin(str, path);
	free(str);
	str = ft_strjoin(tmp, ": No such file or directory\n");
	free(tmp);
	if (fd == 1)
		write(2, str, str_len(str));
	else
		write(fd, str, str_len(str));
	free(str);
	shell->cmd_status = FAILED;
}

void	exit_free()
{
	free_all_env(ret_shell_pointeur(0));
	wrdestroy();
	exit(1);
}

void	cd_move_oldpwd(char *tmp, char *path, t_shell *shell, char *savepwd)
{
	tmp = ft_strdup("export OLDPWD=\"");
	if (!tmp)
		exit_free(shell);
	path = ft_strjoin(tmp, savepwd);
	if (!path)
		exit_free(shell);
	free(tmp);
	tmp = ft_strjoin(path, "\"");
	if (!tmp)
		exit_free(shell);
	free(path);
	shell->parse = shell->save;
	free_parse_things(shell->parse);
	shell->parse = NULL;
	ft_scan_line(tmp, &shell->tokens, shell);
	get_exec_list(&shell->tokens, &shell->parse);
	shell->save = shell->parse;
	if (shell->parse)
		exec_one_cmd(shell);
	free(tmp);
	free(savepwd);
}

void	cd_move_pwd(char *tmp, char *path, t_shell *shell, char *savepwd)
{
	savepwd = ret_pwd();
	tmp = ft_strdup("export PWD=\"");
	if (!tmp)

	path = ft_strjoin(tmp, savepwd);
	free(tmp);
	tmp = ft_strjoin(path, "\"");
	free(path);
	shell->parse = shell->save;
	free_parse_things(shell->parse);
	shell->parse = NULL;
	ft_scan_line(tmp, &shell->tokens, shell);
	get_exec_list(&shell->tokens, &shell->parse);
	shell->save = shell->parse;
	if (shell->parse)
		exec_one_cmd(shell);
	free(tmp);
	free(savepwd);
	shell->cmd_status = SUCESS;
}

void	cd(t_list *lst, t_shell *shell, t_fd fd)
{
	DIR		*rep;
	char	*path;
	char	*tmp;
	t_list	*exec;
	char	*savepwd;

	exec = lst->lst_struct->exec;
	if (exec->next == NULL)
	{
		shell->cmd_status = SUCESS;
		return ;
	}
	savepwd = ret_pwd();
	path = exec->next->content.word;
	if (path[0] == '~')
		cd_tild_inpath(path, shell);
	rep = opendir(path);
	chdir(path);
	if (rep == NULL)
	{
		cd_exception(shell, path, fd);
		free(savepwd);
		return ;
	}
	closedir(rep);
	cd_move_oldpwd(tmp, path, shell, savepwd);
	cd_move_pwd(tmp, path, shell, savepwd);
}
