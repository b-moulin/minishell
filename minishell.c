#include "minishell.h"
#include "get_next_line.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

int		g_normal_shell;

void	init_states(t_state *state)
{
	state->reading_word = 1;
	state->d_quoted_word = 0;
	state->s_quoted_word = 0;
	state->dollar = 0;
}

int	s_quoted_word(char *line, t_list **lst, int i)
{
	t_list	*new;

	while (line[i] && line[i] != '\'')
	{
		new = ft_lstnew(NULL, line[i]);
		new->flag = NONE;
		ft_lstadd_back(lst, new);
		i++;
	}
	return (i++);
}

int	d_quoted_word(char *line, t_tokens *tokens, int i, char **envp)
{
	t_list	*new;

	while (line[i] && line[i] != '\"')
	{
		while (line[i] && line[i] != '$' && line[i] != '\"')
		{
			new = ft_lstnew(NULL, line[i]);
			new->flag = NONE;
			ft_lstadd_back(&tokens->temp, new);
			i++;
		}
		if (line[i] == '$')
		{
			if (tokens->temp)
				from_lst_a_to_lst_b(&tokens->temp, &tokens->words);
			i++;
			i = there_is_env_var(line, i, tokens, envp);
		}
	}
	return (i++);
}

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
	while (new->lst_struct->redir)
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
		}
		else
		{
			shell->read_fd = red_type[red](name);
		}
		if (ret_fd == -1 || shell->read_fd == -1)
			return (-1);
		count++;
		// printf("name %s\n", name);
		if (new->lst_struct->redir)
			new->lst_struct->redir = new->lst_struct->redir->next;
	}
	// printf("ret_fd %d\n", ret_fd);
	return (ret_fd);
}

int		get_fd(t_list **lst, t_shell *shell)
{
	t_list	*new;
	t_list	*first_redir;
	int		fd;

	new = *lst;
	// printf("------redir------\n");
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
				rl_redisplay();
				printf("minishell			 \nminishell ");
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

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	int			result;
	t_tokens	tokens;
	t_list		*parse;
	t_list		*save;
	int			builtin;
	t_fd		fd;
	t_fd		zero_fd;
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
	// BAPTISTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	t_shell	 *shell;
	char		*cmd;

	zero_fd = dup(0);
	g_normal_shell = 1;
	if (argc > 1 || argv[1])
		return (0);
	signal(SIGINT,  ctrl_c);
	cmd = NULL;
	shell = malloc(sizeof(t_shell));
	init_env(envp, shell);
	shell->history[0] = NULL;
	parse = NULL;
	result = 1;
	builtin = 0;
	fd = 1;
	while (1)
	{
		dup2(zero_fd, 0);
		// write(1, "minishell# ", str_len("minishell# "));
		if (fd != -1)
			cmd = readline("minishell ");
		else
			cmd = ft_strdup(rl_line_buffer);
		// printf("=====CMD===== %s\n", cmd);
		if (cmd == 0) // Ctrl-D ==> exit the shell
			exit(0);
		add_history(cmd);
		shell->history[tab_size(shell->history) + 1] = NULL;
		shell->history[tab_size(shell->history)] = ft_strdup(cmd);
		ft_scan_line(cmd, &tokens, shell->env);
		get_exec_list(&tokens, &parse);
		if (tokens.words)
			ft_lstclear(&tokens.words);
		save = parse;
		if (parse)
		{
			fd = get_fd(&parse, shell);
			if (fd != -1)
			{
				if (shell->read_fd != -1)
				{
					dup2(shell->read_fd, 0);
				}
				builtin = is_it_a_builtin(parse);
				if (builtin == -1)
					red_builtin[8](parse, shell, fd);
				else
					red_builtin[builtin](parse, shell, fd);
				if (fd > 1)
					close(fd);
				parse = parse->next;
				if (shell->read_fd != -1)
					close(shell->read_fd);
				shell->read_fd = -1;
			}
		}
		parse = save;
		free_parse_things(parse);
		parse = NULL;
		// printf("\n\n\n\nhere\n\n\n\n");
		// printf("line = [%s]\n", cmd);
	}
	// FIN BAPTISTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	parse = NULL;
	result = 1;
	builtin = 0;
	// printf("%d %d\n", 10 % 2, 11 % 2);
	while (result == 1)
	{
		result = get_next_line(0, &line);
		ft_scan_line(line, &tokens, envp);
		get_exec_list(&tokens, &parse);
		fd = get_fd(&parse, shell);
		if (tokens.words)
			ft_lstclear(&tokens.words);
		if (parse)
		{
			builtin = is_it_a_builtin(parse);
			// printf("builtin = %d\n", builtin);
			free_parse_things(parse);
		}
		parse = NULL;
		// printf("line = [%s]\n", line);
		if (builtin == -1)
			red_builtin[8](parse, shell, fd);
		else
			red_builtin[8](parse, shell, fd);
	}
	if (result == -1)
		return (-1);
	return (0);
}
