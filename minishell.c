#include "minishell.h"
#include "get_next_line.h"
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

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

int	d_quoted_word(char *line, t_list **lst, t_list **wrds_lst, int i)
{
	t_list	*new;

	while (line[i] && line[i] != '\"')
	{
		while (line[i] && line[i] != '$' && line[i] != '\"')
		{
			new = ft_lstnew(NULL, line[i]);
			new->flag = NONE;
			ft_lstadd_back(lst, new);
			i++;
		}
		if (line[i] == '$')
		{
			if (*lst)
				from_lst_a_to_lst_b(lst, wrds_lst);
			i++;
			i = there_is_env_var(line, i, lst, wrds_lst);
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
	// printf("HIREEEEEE %s\n", name);
	fd = open(name, O_RDONLY | O_CREAT, S_IRWXU);
	// printf("fd %d\n", fd);
	return (fd);
}

int		double_right(char	*name)
{
	int		fd;
	fd = open(name, O_APPEND, O_RDONLY | O_CREAT, S_IRWXU);
	return (fd);
}

int		left(char	*name)
{
	int		fd;
	int		ret_gnl;
	char	*line;

	fd = open(name, S_IRUSR);
	ret_gnl = 1;
	line = 0;
	if (fd < 0)
	{
		printf("error !\n"); // faire attention left ne dois pas faire changer le fd de la boucle principale.
		return (1);
	}
	while (ret_gnl > 0)
	{
		ret_gnl = get_next_line(fd, &line);
		ft_putstr_fd(line, 0);
	}
	return (0);
}

int		double_left(char	*name)
{
	free(name); // a faire lors de la fusion
	return (0);
}

enum redirections { RIGHT, DOUBLE_RIGHT, LEFT, DOUBLE_LEFT };

int		redirection_gauche(t_list **lst) // IL FAUT CLOSE LE FD APRES SON UTILISATION !!!
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
		if (count % 2 == 0)
		{
			if (new->lst_struct->redir->content.word[0] == '>' && new->lst_struct->redir->content.word[1] == 0)
				red = RIGHT;
			if (new->lst_struct->redir->content.word[0] == '>' && new->lst_struct->redir->content.word[1] == '>' && new->lst_struct->redir->content.word[1] == 0)
				red = DOUBLE_RIGHT;
			if (new->lst_struct->redir->content.word[0] == '<' && new->lst_struct->redir->content.word[1] == 0)
				red = LEFT;
			if (new->lst_struct->redir->content.word[0] == '<' && new->lst_struct->redir->content.word[1] == '<' && new->lst_struct->redir->content.word[1] == 0)
				red = DOUBLE_LEFT;
		}
		new->lst_struct->redir = new->lst_struct->redir->next;
		name = ft_strdup(new->lst_struct->redir->content.word);
		// au cas ou on a plusieurs redirecctions (echo bonjour >a>>b>>c)
		if (ret_fd != 1)
			close(ret_fd);
		// tableau de pointeur sur fonction
		// printf("red %d\n", red_type[red]);
		ret_fd = red_type[red](name);
		count++;
		// printf("name %s\n", name);
		if (new->lst_struct->redir)
			new->lst_struct->redir = new->lst_struct->redir->next;
	}
	return (ret_fd);
}

void	get_fd(t_list **lst)
{
	t_list	*new;
	t_list	*first_redir;

	new = *lst;
	printf("------redir------\n");
	while (new)
	{
		first_redir = new->lst_struct->redir;
		while (new->lst_struct->redir)
		{
			printf("[%s]\n", new->lst_struct->redir->content.word);
			new->lst_struct->redir = new->lst_struct->redir->next;
		}
		new->lst_struct->redir = first_redir;
		new = new->next;
		printf("------\n");
	}
	new = *lst;
	redirection_gauche(lst);
	// if (new && (new->lst_struct->redir->content.word[0] == '>' && new->lst_struct->redir->content.word[1] == 0 || new->lst_struct->redir->content.word[0] == '>' && new->lst_struct->redir->content.word[1] == '>' && new->lst_struct->redir->content.word[2] == 0))
	// 	redirection_gauche(lst);
	// else if 
}

void    ctrl_c(int sig)
{
    char    *cmd;
    int     i;

    cmd = ft_strdup(rl_line_buffer);
    i = str_len(cmd);
    if (sig == SIGINT) // ctrl-C
    {
		// printf("cmd |%s|---------\n", cmd);
		if (cmd[0] == 0)
		{
			rl_redisplay();
			printf("minishell             \nminishell ");
			// printf("  \nHEREEEEEEEEminishell ");
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
    free(cmd);
}

// int     main(int argc, char **argv, char **envp)
// {
//     t_shell     *shell;
//     char        *cmd;
//     char        t[1];
//     int         i=0;

//     signal(SIGINT,  ctrl_c);
//     cmd = NULL;
//     shell = malloc(sizeof(t_shell));
//     init_env(envp, shell);
//     shell->history[0] = NULL;
// }

////////////////////////////////////////////////////////////////////////

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	int			result;
	t_tokens	tokens;
	t_list		*parse;
	int			builtin;

	// BAPTISTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	t_shell     *shell;
    char        *cmd;

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

	while (1)
    {
        // write(1, "minishell# ", str_len("minishell# "));
        cmd = readline("minishell ");
        if (cmd == 0) // Ctrl-D ==> exit the shell
            exit(0);
        shell->history[tab_size(shell->history) + 1] = NULL;
        shell->history[tab_size(shell->history)] = ft_strdup(cmd);
		ft_scan_line(cmd, &tokens);
		get_exec_list(&tokens, &parse);
		get_fd(&parse);
		if (tokens.words)
			ft_lstclear(&tokens.words);
		if (parse)
		{
			builtin = is_it_a_builtin(parse);
			printf("builtin = %d\n", builtin);
			free_parse_things(parse);
		}
		parse = NULL;
		printf("line = [%s]\n", cmd);
    }

	// FIN BAPTISTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	parse = NULL;
	result = 1;
	builtin = 0;
	// printf("%d %d\n", 10 % 2, 11 % 2);
	while (result == 1)
	{
		result = get_next_line(0, &line);
		ft_scan_line(line, &tokens);
		get_exec_list(&tokens, &parse);
		get_fd(&parse);
		if (tokens.words)
			ft_lstclear(&tokens.words);
		if (parse)
		{
			builtin = is_it_a_builtin(parse);
			printf("builtin = %d\n", builtin);
			free_parse_things(parse);
		}
		parse = NULL;
		printf("line = [%s]\n", line);
	}
	if (result == -1)
		return (-1);
	return (0);
}
