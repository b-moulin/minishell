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

#include "minishell.h"
#include "get_next_line.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int		g_normal_shell;

void	ft_putstr_fd(char *s, int fd)
{
	size_t	i;

	i = 0;
	if (!(!s || !fd))
	{
		while (s[i])
		{
			write(fd, &s[i], 1);
			i++;
		}
	}
}

int	right(char	*name)
{
	int		fd;

	close(open(name, O_RDONLY | O_CREAT | O_TRUNC, S_IRWXU));
	fd = open(name, O_TRUNC | O_RDWR);
	return (fd);
}

int	double_right(char	*name)
{
	int		fd;

	close(open(name, O_WRONLY | O_CREAT | O_APPEND | O_CREAT, S_IRWXU));
	fd = open(name, O_WRONLY | O_CREAT | O_APPEND);
	return (fd);
}

int	left(char	*name)
{
	int		fd;

	fd = open(name, O_CREAT, S_IRUSR);
	if (fd < 0)
	{
		printf("error !\n");
		return (1);
	}
	return (fd);
}

void	middle_double_left(char *name, t_dl *dl)
{
	rl_replace_line("", 0);
	rl_redisplay();
	dl->cmd = readline("> ");
	while (g_normal_shell == 0 && ft_strcmp(dl->cmd, name) != 0)
	{
		// fonction emma pour replace env_var
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
	dl->fd = left("/tmp/minishell.tmp");
	return (dl->fd);
}

void	init_redirection_gauche(t_list **lst, t_red_gauche	*rg)
{
	rg->new = *lst;
	rg->count = 0;
	rg->name = 0;
	rg->ret_fd = 1;
	rg->red_type[0] = &right;
	rg->red_type[1] = &double_right;
	rg->red_type[2] = &left;
	rg->red_type[3] = &double_left;
}

int	loop_redirection_gauche(t_list **lst, t_shell *shell, t_red_gauche	*rg)
{
	rg->new->lst_struct->redir = rg->new->lst_struct->redir->next;
	rg->name = ft_strdup(rg->new->lst_struct->redir->content.word);
	if (rg->red == RIGHT || rg->red == DOUBLE_RIGHT)
	{
		rg->ret_fd = rg->red_type[rg->red](rg->name);
		if (rg->ret_fd == -1)
			return (-1);
	}
	else
	{
		shell->read_fd = rg->red_type[rg->red](rg->name);
		if (shell->read_fd == -1)
			return (-1);
	}
	return (0);
}

void	firstpart_redirection_gauche(t_list **lst,
	t_shell *shell, t_red_gauche	*rg)
{
	if (rg->count % 2 == 0 && rg->new->lst_struct->redir->content.word)
	{
		if (rg->new->lst_struct->redir->content.word[0]
			&& rg->new->lst_struct->redir->content.word[0] == '>'
			&& rg->new->lst_struct->redir->content.word[1] == 0)
			rg->red = RIGHT;
		if (rg->new->lst_struct->redir->content.word[0]
			&& rg->new->lst_struct->redir->content.word[0] == '>'
			&& rg->new->lst_struct->redir->content.word[1]
			&& rg->new->lst_struct->redir->content.word[1] == '>'
			&& rg->new->lst_struct->redir->content.word[2] == 0)
			rg->red = DOUBLE_RIGHT;
		if (rg->new->lst_struct->redir->content.word[0]
			&& rg->new->lst_struct->redir->content.word[0] == '<'
			&& rg->new->lst_struct->redir->content.word[1] == 0)
			rg->red = LEFT;
		if (rg->new->lst_struct->redir->content.word[0]
			&& rg->new->lst_struct->redir->content.word[0] == '<'
			&& rg->new->lst_struct->redir->content.word[1]
			&& rg->new->lst_struct->redir->content.word[1] == '<'
			&& rg->new->lst_struct->redir->content.word[2] == 0)
			rg->red = DOUBLE_LEFT;
	}
}

int	redirection_gauche(t_list **lst, t_shell *shell)
{
	t_red_gauche	*rg;

	rg = malloc(sizeof(t_red_gauche));
	if (!rg)
		exit(0);
	init_redirection_gauche(lst, rg);
	while (rg->new && rg->new->lst_struct && rg->new->lst_struct->redir)
	{
		firstpart_redirection_gauche(lst, shell, rg);
		if (loop_redirection_gauche(lst, shell, rg) != 0)
			return (-1);
		rg->count++;
		if (rg->new->lst_struct->redir)
			rg->new->lst_struct->redir = rg->new->lst_struct->redir->next;
	}
	return (rg->ret_fd);
}

int	get_fd(t_list **lst, t_shell *shell)
{
	t_list	*new;
	t_list	*first_redir;
	int		fd;

	new = *lst;
	while (new)
	{
		first_redir = new->lst_struct->redir;
		while (new->lst_struct->redir)
			new->lst_struct->redir = new->lst_struct->redir->next;
		new->lst_struct->redir = first_redir;
		new = new->next;
	}
	new = *lst;
	fd = redirection_gauche(lst, shell);
	return (fd);
}

void	middle_cc(t_cc *cc)
{
	if (cc->cmd[0] == 0)
	{
		printf("minishell2                              \nminishell3 ");
		rl_redisplay();
		rl_replace_line("", 0);
	}
	else
	{
		rl_replace_line("", 0);
		rl_redisplay();
		if (str_len(cc->cmd) <= 11)
			printf("minishell4 ");
		printf("%s", cc->cmd);
		printf("  \n");
		printf("minishell5 ");
	}
}

void	ctrl_c(int sig)
{
	t_cc	*cc;

	cc = malloc(sizeof(t_cc));
	if (!cc)
		exit(1);
	cc->cmd = ft_strdup(rl_line_buffer);
	cc->i = str_len(cc->cmd);
	if (sig == SIGINT)
	{
		if (g_normal_shell == 0)
		{
			rl_replace_line("", 0);
			rl_redisplay();
			if (str_len(cc->cmd) <= 11)
				printf("> ");
			printf("%s", cc->cmd);
			printf("  \n");
			printf("minishell1 ");
		}
		else
			middle_cc(cc);
		g_normal_shell = 1;
	}
	free(cc->cmd);
}

void	exec_one_cmd(t_shell *shell)
{
	void		(*red_builtin[9])(t_list *, t_shell *, t_fd);

	red_builtin[0] = &echo;
	red_builtin[1] = &cd;
	red_builtin[2] = &pwd;
	red_builtin[3] = &export;
	red_builtin[4] = &unset;
	red_builtin[5] = &env;
	red_builtin[6] = &exit_cmd;
	red_builtin[7] = 0;
	red_builtin[8] = &doo_execve;
	shell->fd = get_fd(&shell->parse, shell);
	if (shell->fd != -1)
	{
		if (shell->read_fd > 0)
			dup2(shell->read_fd, 0);
		shell->builtin = is_it_a_builtin(shell->parse);
		if (shell->builtin == -1)
			red_builtin[8](shell->parse, shell, shell->fd);
		else
			red_builtin[shell->builtin](shell->parse, shell, shell->fd);
		shell->parse = shell->parse->next;
	}
	if (shell->fd > 1)
		close(shell->fd);
	if (shell->read_fd != -1)
		close(shell->read_fd);
	shell->read_fd = -1;
}

void	do_pipe_cmd(t_shell *shell)
{
	int		nb_pipes;
	int		pipes_set;
	int		exit_status;
	int		count;

	pipes_set = 0;
	count = 0;
	exit_status = 0;
	nb_pipes = ft_lstsize(shell->parse) - 1;
	if (nb_pipes <= 0)
		return ;
	int		pipe_fd[nb_pipes][2];
	int		cpid[nb_pipes + 1];
	while (pipes_set != nb_pipes)
	{
		pipe(pipe_fd[pipes_set]);
		pipes_set++;
	}
	pipes_set = 0;

	while (count != nb_pipes + 1)
	{
		cpid[count] = fork();
		if (!cpid[count] && count == 0)
		{
			close(pipe_fd[0][0]);
			dup2(pipe_fd[0][1],1);
			exec_one_cmd(shell);
			exit(0);
		}
		else if (!cpid[count] && count == nb_pipes)
		{
			close(pipe_fd[nb_pipes - 1][1]);
			dup2(pipe_fd[nb_pipes - 1][0], 0);
			exec_one_cmd(shell);
			exit(0);
		}
		else if (!cpid[count])
		{
			close(pipe_fd[count][0]);
			dup2(pipe_fd[count - 1][0], 0);
			close(pipe_fd[count - 1][1]);
			dup2(pipe_fd[count][1], 1);
			exec_one_cmd(shell);
			exit(0);
		}
		if (count < nb_pipes)
			close (pipe_fd[count][1]);
		shell->parse = shell->parse->next;
		count++;
	}
	count = 0;
	while (count != nb_pipes + 1)
	{
		do_waitpid(shell, cpid[count], &exit_status);
		count++;
	}
}


int	main(int argc, char **argv, char **envp)
{
	t_shell	 	*shell;
	char		*cmd;

	g_normal_shell = 1;
	if (argc > 1 || argv[1])
		return (0);
	signal(SIGINT,  ctrl_c);
	cmd = NULL;
	shell = malloc(sizeof(t_shell));
	init_env(envp, shell);
	shell->history[0] = NULL;
	shell->parse = NULL;
	shell->builtin = 0;
	shell->fd = 1;
	shell->zero_fd = dup(0);
	shell->un_fd = dup(1);
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
		add_history(cmd);
		shell->history[tab_size(shell->history) + 1] = NULL;
		shell->history[tab_size(shell->history)] = ft_strdup(cmd);
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
}
