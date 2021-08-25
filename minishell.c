#include "minishell.h"
#include "get_next_line.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int		g_normal_shell;

////////////////////////////////////////////////////////////////////////

// EXEMPLE DE LISTE POUR echo bonjour >a>>b : [>][a][>>][b]

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

int		right(char	*name)
{
	int		fd;
	close(open(name, O_RDONLY | O_CREAT | O_TRUNC, S_IRWXU));
	fd = open(name, O_TRUNC | O_RDWR);
	return (fd);
}

int		double_right(char	*name)
{
	int		fd;
	close(open(name, O_WRONLY | O_CREAT | O_APPEND | O_CREAT, S_IRWXU));
	fd = open(name, O_WRONLY | O_CREAT | O_APPEND);
	return (fd);
}

int		left(char	*name)
{
	int		fd;

	fd = open(name, O_CREAT, S_IRUSR);
	if (fd < 0)
	{
		printf("error !\n"); // faire attention left ne dois pas faire changer le fd de la boucle principale.
		return (1);
	}
	return (fd);
}

int		double_left(char	*name)
{
	char	*cmd;
	int		fd;

	cmd = 0;
	g_normal_shell = 0;
	close(open("/tmp/minishell.tmp", O_RDONLY | O_CREAT | O_TRUNC, S_IRWXU));
	fd = open("/tmp/minishell.tmp", O_TRUNC | O_RDWR);
	// printf("here %d\n", fd); 
	if (fd < 0)
		return (-1);
	rl_replace_line("", 0);
	rl_redisplay();
	// printf("> ");
	cmd = readline("> ");
	while (g_normal_shell == 0 && ft_strcmp(cmd, name) != 0)
	{
		// ft_putstr_fd("here", 1);
		ft_putstr_fd(cmd, fd);
		ft_putstr_fd("\n", fd);
		free(cmd);
		cmd = 0;
		rl_replace_line("", 0);
		rl_redisplay();
		cmd = readline("> ");
	}
	// la bonne commande a traiter (et non celle d'avant, potentiellement appeler la fonction qui traite les commandes puisque ctrl-c annule l'interaction).
	if (g_normal_shell == 1)
	{
		rl_line_buffer = cmd;
		return (-1);
	}
	free(cmd);
	cmd = 0;
	g_normal_shell = 1;
	close(fd);
	fd = left("/tmp/minishell.tmp");
	return (fd);
}

enum redirections { RIGHT, DOUBLE_RIGHT, LEFT, DOUBLE_LEFT };

int		redirection_gauche(t_list **lst, t_shell *shell) // IL FAUT CLOSE LE FD APRES SON UTILISATION !!!
{
	t_list				*new;
	enum redirections	red;
	char				*name;
	int					count;
	int					ret_fd;
	int					(*red_type[4])(char *);

	new = *lst;
	count = 0;
	name = 0;
	ret_fd = 1;
	red_type[0] = &right;
	red_type[1] = &double_right;
	red_type[2] = &left;
	red_type[3] = &double_left;
	while (new && new->lst_struct && new->lst_struct->redir)
	{
		if (count % 2 == 0 && new->lst_struct->redir->content.word)
		{
			if (new->lst_struct->redir->content.word[0] && new->lst_struct->redir->content.word[0] == '>' && new->lst_struct->redir->content.word[1] == 0)
				red = RIGHT;
			if (new->lst_struct->redir->content.word[0] && new->lst_struct->redir->content.word[0] == '>' && new->lst_struct->redir->content.word[1] && new->lst_struct->redir->content.word[1] == '>' && new->lst_struct->redir->content.word[2] == 0)
				red = DOUBLE_RIGHT;
			if (new->lst_struct->redir->content.word[0] && new->lst_struct->redir->content.word[0] == '<' && new->lst_struct->redir->content.word[1] == 0)
				red = LEFT;
			if (new->lst_struct->redir->content.word[0] && new->lst_struct->redir->content.word[0] == '<' && new->lst_struct->redir->content.word[1] && new->lst_struct->redir->content.word[1] == '<' && new->lst_struct->redir->content.word[2] == 0)
				red = DOUBLE_LEFT;
			// printf("HERERHRHEHHEHEHHEH %c %c %c\n", new->lst_struct->redir->content.word[0], new->lst_struct->redir->content.word[1]);
		}
		new->lst_struct->redir = new->lst_struct->redir->next;
		name = ft_strdup(new->lst_struct->redir->content.word);
		// au cas ou on a plusieurs redirecctions (echo bonjour >a>>b>>c)
		// if (ret_fd != 1)
		// 	close(ret_fd);
		// tableau de pointeur sur fonction
		// printf("red %d\n", red_type[red]);
		if (red == RIGHT || red == DOUBLE_RIGHT)
		{
			ret_fd = red_type[red](name);
			if (ret_fd == -1)
				return (-1);
		}
		else
		{
			shell->read_fd = red_type[red](name);
			if (shell->read_fd == -1)
				return (-1);
		}
		count++;
		// printf("name %s\n", name);
		if (new->lst_struct->redir)
			new->lst_struct->redir = new->lst_struct->redir->next;
	}
	return (ret_fd);
}

int		get_fd(t_list **lst, t_shell *shell)
{
	t_list	*new;
	t_list	*first_redir;
	int		fd;

	new = *lst;
	while (new)
	{
		first_redir = new->lst_struct->redir;
		while (new->lst_struct->redir)
		{
			// printf("[%s]\n", new->lst_struct->redir->content.word);
			new->lst_struct->redir = new->lst_struct->redir->next;
		}
		new->lst_struct->redir = first_redir;
		new = new->next;
		// printf("------\n");
	}
	new = *lst;
	fd = redirection_gauche(lst, shell);
	// if (fd < 0)
		// return (1);
	return (fd);
}

void	ctrl_c(int sig)
{
	char	*cmd;
	int	 i;

	cmd = ft_strdup(rl_line_buffer);
	i = str_len(cmd);
	if (sig == SIGINT) // ctrl-C
	{
		if (g_normal_shell == 0)
		{
			rl_replace_line("", 0);
			rl_redisplay();
			if (str_len(cmd) <= 11)
				printf("> ");
			printf("%s", cmd);
			printf("  \n");
			printf("minishell ");
		}
		else
		{
			if (cmd[0] == 0)
			{
				printf("minishell                              \nminishell ");
				rl_redisplay();
				rl_replace_line("", 0);
			}
			else
			{
				rl_replace_line("", 0);
				rl_redisplay();
				if (str_len(cmd) <= 11)
					printf("minishell ");
				printf("%s", cmd);
				printf("  \n");
				printf("minishell ");
			}
		}
		g_normal_shell = 1;
	}
	free(cmd);
}

void	exec_one_cmd(t_shell *shell)
{
	//TABLEAU DE POINTEUR SUR FONCTION
	void		(*red_builtin[9])(t_list *, t_shell *, t_fd);

	red_builtin[0] = &echo;
	red_builtin[1] = &cd;
	red_builtin[2] = &pwd;
	red_builtin[3] = &export;
	red_builtin[4] = &unset;
	red_builtin[5] = &env;
	red_builtin[6] = &exit_cmd;
	red_builtin[7] = &print_history;
	red_builtin[8] = &doo_execve;
	// FIN TABLEAU DE POINTEUR SUR FONCTION

	shell->fd = get_fd(&shell->parse, shell);
	// while (shell->fd == -1)
	// 	shell->fd = get_fd(&shell->parse, shell);
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
	// int		cpid;
	int		exit_status;
	int		count;

	pipes_set = 0;
	count = 0;
	exit_status = 0;
	// cpid = 0;
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
		// dprintf(2, "")
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
			// printf("minishett ");
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
		if (cmd == 0) // Ctrl-D ==> exit the shell
		{
			free_all_env(shell);
			// wrdestroy();
			exit(0);
		}
		add_history(cmd);
		shell->history[tab_size(shell->history) + 1] = NULL;
		shell->history[tab_size(shell->history)] = ft_strdup(cmd);
		ft_scan_line(cmd, &shell->tokens, shell);
		get_exec_list(&shell->tokens, &shell->parse);
		if (shell->tokens.words)
			ft_lstclear(&shell->tokens.words);
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
	}
	// FIN BAPTISTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}
