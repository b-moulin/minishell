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

void	exit_free(void)
{
	free_all_env(ret_shell_pointeur(0));
	wrdestroy();
	exit(1);
}

void	exit_free_ctrl_d(void)
{
	free_all_env(ret_shell_pointeur(0));
	exit(0);
}

void	cd_move_oldpwd(char *tmp, char *path, t_shell *shell, char *savepwd)
{
	tmp = ft_strdup("export OLDPWD=\"");
	if (!tmp)
		exit_free();
	path = ft_strjoin(tmp, savepwd);
	if (!path)
		exit_free();
	free(tmp);
	tmp = ft_strjoin(path, "\"");
	if (!tmp)
		exit_free();
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
		exit_free();
	path = ft_strjoin(tmp, savepwd);
	if (!path)
		exit_free();
	free(tmp);
	tmp = ft_strjoin(path, "\"");
	if (!tmp)
		exit_free();
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
