/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efarin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 14:23:24 by efarin            #+#    #+#             */
/*   Updated: 2021/08/26 14:23:27 by efarin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "get_next_line.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

void	middle_double_left(char *name, t_dl *dl)
{
	t_shell	*shell;

	shell = ret_shell_pointeur(NULL);
	rl_replace_line("", 0);
	rl_redisplay();
	dl->cmd = readline("> ");
	while (g_normal_shell == 0 && ft_strcmp(dl->cmd, name) != 0)
	{
		dl->cmd = parse_hirdoc_str(dl->cmd, shell);
		ft_putstr_fd(dl->cmd, dl->fd);
		ft_putstr_fd("\n", dl->fd);
		free(dl->cmd);
		dl->cmd = 0;
		rl_replace_line("", 0);
		rl_redisplay();
		dl->cmd = readline("> ");
	}
}

int	double_left(char	*name)
{
	t_dl	*dl;
	int		ret;

	dl = malloc(sizeof(t_dl));
	if (!dl)
		exit(1);
	dl->cmd = 0;
	g_normal_shell = 0;
	close(open("/tmp/minishell.tmp", O_RDONLY | O_CREAT | O_TRUNC, S_IRWXU));
	dl->fd = open("/tmp/minishell.tmp", O_TRUNC | O_RDWR);
	if (dl->fd < 0)
		return (-1);
	middle_double_left(name, dl);
	if (g_normal_shell == 1)
	{
		rl_line_buffer = dl->cmd;
		return (-1);
	}
	free(dl->cmd);
	dl->cmd = 0;
	g_normal_shell = 1;
	close(dl->fd);
	ret = left("/tmp/minishell.tmp");
	free(dl);
	return (ret);
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
	ret_shell_pointeur(shell);
	return (shell);
}

void	main_loop_content(t_shell *shell, char *cmd)
{
	add_history(cmd);
	ft_scan_line(cmd, &shell->tokens, shell);
	get_exec_list(&shell->tokens, &shell->parse);
	shell->save = shell->parse;
	if (shell->parse && shell->parse->next)
	{
		do_pipe_cmd(shell);
	}
	else if (shell->parse)
	{
		exec_one_cmd(shell);
	}
	shell->parse = shell->save;
	free_parse_things(shell->parse);
	shell->parse = NULL;
	free(cmd);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell		*shell;
	char		*cmd;

	g_normal_shell = 1;
	if (argc > 1 || argv[1])
		return (0);
	cmd = NULL;
	signal(SIGINT, ctrl_c);
	shell = init_main(envp);
	while (1)
	{
		dup2(shell->un_fd, 1);
		dup2(shell->zero_fd, 0);
		if (shell->fd != -1)
			cmd = readline("minishell ");
		else
			cmd = ft_strdup(rl_line_buffer);
		if (cmd == 0)
		{
			free_all_env(shell);
			exit(0);
		}
		main_loop_content(shell, cmd);
	}
}
