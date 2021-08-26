/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efarin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 14:27:46 by efarin            #+#    #+#             */
/*   Updated: 2021/08/26 14:27:48 by efarin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "get_next_line.h"
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <stdio.h>
# include <sys/types.h>
# include <limits.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

# define SUCESS 0
# define FAILED 127
# define NONE 0
# define SPACEE 1
# define DOLLAR 2
# define SPECIAL 3

int			g_normal_shell;

typedef int	t_fd;
enum		e_redirections { RIGHT, DOUBLE_RIGHT, LEFT, DOUBLE_LEFT };

typedef enum e_bool
{
	FALSE,
	TRUE,
}	t_bool;

typedef struct s_lst
{
	void			*content;
	struct s_lst	*next;
}				t_lst;

typedef struct s_dl
{
	char	*cmd;
	int		fd;
}				t_dl;

typedef struct s_cc
{
	char	*cmd;
	int		i;
}				t_cc;

typedef struct s_lst_content
{
	char	c;
	char	*word;
}				t_lst_content;

typedef struct s_list
{
	int				pipe_fd[2];
	t_lst_content	content;
	struct s_list	*next;
	struct s_parse	*lst_struct;
	int				flag;
	int				j;
	struct s_list	*redir_head;
}				t_list;

typedef struct s_red_gauche
{
	t_list				*new;
	enum e_redirections	red;
	char				*name;
	int					count;
	int					ret_fd;
	int					(*red_type[4])(char *);
}				t_red_gauche;

typedef struct s_parse
{
	t_list		*exec;
	t_list		*redir;
}				t_parse;

typedef struct s_strnstr
{
	size_t	i;
	size_t	j;
	size_t	p;
}				t_strnstr;

typedef struct s_envinit
{
	int		i;
	size_t	j;
	char	*env_name;
	char	*env_arg;
	char	*tmp;
	char	*save;
}				t_envinit;

typedef struct s_export
{
	char	*tmp;
	char	*str;
	size_t	i;
	size_t	save;
	char	*token;
	size_t	count;
	t_fd	fd;
}				t_export;

typedef struct s_exarg
{
	int		i;
	char	**new_env;
	char	*tmp;
}				t_exarg;

typedef struct s_exargall
{
	int		i;
	char	**new_env;
	char	*tmp;
	char	*save;
}				t_exargall;

typedef struct s_exprt
{
	t_list	*exec;
	char	*name;
	char	*arg;
	int		i;
	char	*tmp;
}				t_exprt;

typedef struct s_exve
{
	char	**path;
	char	*envarg;
	char	*all_path;
	size_t	i;
	int		pid;
	int		exve;
	int		exit_status;
}				t_exve;

typedef struct s_state
{
	int		reading_word;
	int		s_quoted_word;
	int		d_quoted_word;
	int		dollar;
	int		pipe;
	char	redir;
	int		cmd_state;
}				t_state;

typedef struct s_tokens
{
	t_state		state;
	t_list		*temp;
	t_list		*words;
}				t_tokens;

typedef struct s_shell
{
	int			un_fd;
	int			zero_fd;
	char		**env;
	char		**env_all;
	int			cmd_status;
	int			read_fd;
	t_tokens	tokens;
	t_list		*parse;
	t_list		*save;
	int			builtin;
	t_fd		fd;
}				t_shell;

typedef struct s_pipe
{
	int		nb_pipes;
	int		pipes_set;
	int		exit_status;
	int		count;
	int		pipe_fd[1000][2];
	int		cpid[1000 + 1];
}				t_pipe;

typedef struct s_exec
{
	void		(*red_builtin[9])(t_list *, t_shell *, t_fd);
}				t_exec;

void		ft_lstadd_backBis(t_lst **alst, t_lst *new);
t_lst		*ft_lstlastBis(t_lst *lst);
t_bool		is_valid_env_name(const char *env);
int			find_env_var(const char *tofind, t_shell *shell);
int			find_env_all_var(const char *tofind, t_shell *shell);
void		export_no_name(const char *arg, t_fd fd, t_shell *shell);
void		export_error_token(t_export export,
				const char *name,
				t_shell *shell, const char *arg);
void		cutcut_export_error(t_export export, t_shell *shell,
				const char *name, const char *arg);
void		cut_export_error(t_export export, t_shell *shell,
				const char *name, const char *arg);
t_export	init_export(t_fd fd);
void		export_error(const char *name,
				const char *arg, t_fd fd, t_shell *shell);
void		end_export_add_arg(t_exarg axarg, t_shell *shell,
				const char *name, const char *arg);
void		export_add_arg(const char *name,
				const char *arg, t_shell *shell, t_fd fd);
void		end_export_add_arg_all(const char *arg,
				t_exargall exargall, t_shell *shell);
void		export_add_arg_all(const char *name,
				const char *arg, t_shell *shell);
void		export_all(const char *name,
				const char *arg, t_shell *shell);
void		end_export(t_exprt *exprt, t_shell *shell, t_fd fd);
void		middle_export(t_exprt *exprt);
void		export(t_list *lst, t_shell *shell, t_fd fd);
void		free_all_env(t_shell *shell);
void		free_double_tab(char **tab);
void		do_waitpid(t_shell *shell, pid_t pid, int *i);
int			tab_size(char **tab);
int			find_env_all_var(const char *tofind, t_shell *shell);
char		*ft_strdup(const char *s1);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_strnstr(const char *haystack, const char *needle, size_t len);
size_t		str_len(const char *str);
char		**ft_split(char const *s, char c);
size_t		str_len(const char *str);
char		*get_env_arg(char **env, char *tofind);
int			find_env_var(const char *tofind, t_shell *shell);
void		ft_freee(char **tofree);
char		**ft_split(char const *s, char c);
char		*ft_substr(char const *s, unsigned int start, size_t len);
void		init_env(char **env, t_shell *shell);
long long	ft_atoi(const char *str);
void		ft_putstr_fd(char *s, int fd);
char		*ft_itoa(int n);
void		print_history(t_list *lst, t_shell *shell, t_fd fd);
void		echo(t_list *lst, t_shell *shell, t_fd fd);
void		cd(t_list *lst, t_shell *shell, t_fd fd);
void		env(t_list *lst, t_shell *shell, t_fd fd);
void		exit_cmd(t_list *lst, t_shell *shell, t_fd fd);
void		export(t_list *lst, t_shell *shell, t_fd fd);
void		pwd(t_list *lst, t_shell *shell, t_fd fd);
void		unset(t_list *lst, t_shell *shell, t_fd fd);
void		doo_execve(t_list *lst, t_shell *shell, t_fd fd);
void		do_execve(t_shell *shell,
				const char *command, char **argv, int fd);
void		env_all(t_list *lst, t_shell *shell, t_fd fd);
t_list		*ft_lstnew(char *str, char c);
void		ft_lstadd_back(t_list **alst, t_list *new);
void		ft_lstclear(t_list **lst);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
t_list		*ft_lst_struct_new(void);
int			ft_strcmp(char *s1, char *s2);
char		*ft_strdup(const char *s);
char		*ft_strjoin(char const *s1, char const *s2);
char		*ft_free(char **tab, size_t i);
char		*from_lst_to_str(t_list *a);
int			from_lst_a_to_lst_b(t_list **a, t_list **b);
void		init_states(t_state *state);
int			s_quoted_word(char *line, t_list **lst, int i);
int			d_quoted_word(char *line,
				t_tokens *tokens, int i, t_shell *exec_part);
int			there_is_space(char *line, int i, t_tokens *tokens);
void		join_the_tokens(t_tokens *tokens, t_list **words);
void		ft_scan_line(char *line,
				t_tokens *tokens, t_shell *exec_part);
int			whats_the_state(char *line,
				t_tokens *tokens, int i, t_shell *exec_part);
int			there_is_env_var(char *line,
				int i, t_tokens *tokens, t_shell *exec_part);
int			get_env_var_value(t_list **item, t_list **start, char **envp);
int			there_is_redir(char *line, int i, t_tokens *tokens);
t_list		*remove_a_list_item(t_list **lst, t_list *item);
int			reading_word_state(char *line, int i,
				t_tokens *tokens, t_shell *exec_part);
t_list		*retoken_env_var(char *env_var);
int			check_fd_redir(char *word);
void		free_parse_things(t_list *parse);
void		free_tokens_things(t_tokens *tokens, int error);
int			is_it_a_builtin(t_list *parse);
void		get_exec_list(t_tokens *tokens, t_list **parse);
void		free_words_content(t_tokens *tokens);
void		*wrmalloc(unsigned long size);
void		wrdestroy(void);
void		print_lst_after_parse(t_list *lst);
void		print_lst(t_list *lst);
int			right(char	*name);
int			double_right(char	*name);
int			left(char	*name);
int			double_left(char	*name);
void		middle_double_left(char *name, t_dl *dl);
void		init_redirection_gauche(t_list **lst,
				t_red_gauche	*rg);
int			loop_redirection_gauche(t_list **lst,
				t_shell *shell, t_red_gauche	*rg);
void		firstpart_redirection_gauche(t_list **lst,
				t_shell *shell, t_red_gauche *rg);
int			redirection_gauche(t_list **lst, t_shell *shell);
void		ctrl_c(int sig);
void		exec_one_cmd(t_shell *shell);
void		do_pipe_cmd(t_shell *shell);
int			get_fd(t_list **lst, t_shell *shell);

#endif
