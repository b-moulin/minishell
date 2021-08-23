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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
# include "/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/usr/include/readline/readline.h"
# include "/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/usr/include/readline/history.h"
# include "/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk/usr/include/editline/readline.h"

# define SUCESS 0
# define FAILED 127
# define NONE 0
# define SPACEE 1
# define DOLLAR 2
# define SPECIAL 3

typedef int t_fd;

typedef enum e_bool
{
	FALSE,
    TRUE,
}	t_bool;

typedef struct	s_lst_content
{
	char	c;
	char	*word;
}				t_lst_content;

struct s_parse;

typedef struct s_list
{
	int			pipe_fd[2];
	t_lst_content	content;
	struct s_list	*next;
	struct s_parse	*lst_struct;
	int				flag;
}				t_list;

typedef struct s_lst
{
	void			*content;
	struct s_lst	*next;
}				t_lst;

typedef struct s_parse
{
	t_list		*exec;
	t_list		*redir;
}				t_parse;

typedef struct s_state
{
	int		reading_word;
	int		s_quoted_word;
	int		d_quoted_word;
	int		dollar;
	int		pipe;
	char	redir;
}				t_state;

typedef struct s_tokens
{
	t_state		state;
	t_list		*temp;
	t_list		*words;
}				t_tokens;

typedef struct s_exec
{

}				t_exec;

typedef struct s_shell
{
	int			un_fd;
	int			zero_fd;
    char    	**env;
    char    	**env_all;
    char    	*history[500];
    int     	cmd_status;
	int			read_fd;
	t_tokens	tokens;
	t_list		*parse;
	t_list		*save;
	int			builtin;
	t_fd		fd;
}   t_shell;

void    free_all_env(t_shell *shell);
void    free_double_tab(char **tab);
// void	*wrmalloc(unsigned long size);
// void	wrdestroy(void);


void			do_waitpid(t_shell *shell, pid_t  pid, int *i);

int     		tab_size(char   **tab);
int     		find_env_all_var(const char *tofind, t_shell *shell);
char			*ft_strdup(const char *s1);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_strnstr(const char *haystack, const char *needle, size_t len);
size_t  		str_len(const char *str);
char			**ft_split(char const *s, char c);
size_t  		str_len(const char *str);

char    		*get_env_arg(char **env, char *tofind);

int     		find_env_var(const char *tofind, t_shell *shell);

void    		ft_freee(char    **tofree);
char			**ft_split(char const *s, char c);
char			*ft_substr(char const *s, unsigned int start, size_t len);
void    		init_env(char  **env, t_shell  *shell);
long long		ft_atoi(const char *str);
void			ft_putstr_fd(char *s, int fd);
char    		*ft_itoa(int n);

void    		print_history(t_list *lst, t_shell *shell, t_fd fd);
void    		echo(t_list *lst, t_shell *shell, t_fd fd);
void			cd(t_list *lst, t_shell *shell, t_fd fd);
void			env(t_list *lst, t_shell *shell, t_fd fd);
void			exit_cmd(t_list *lst, t_shell *shell, t_fd fd);
void    		export(t_list *lst, t_shell *shell, t_fd fd);
void    		pwd(t_list *lst, t_shell *shell, t_fd fd);
void    		unset(t_list *lst, t_shell *shell, t_fd fd);
void			doo_execve(t_list *lst, t_shell *shell, t_fd fd);
void    		do_execve(t_shell   *shell, const char *command, char **argv, int fd);
void			env_all(t_list *lst, t_shell *shell, t_fd fd);

t_list			*ft_lstnew(char *str, char c);
void			ft_lstadd_back(t_list **alst, t_list *new);
void			ft_lstclear(t_list **lst);
int				ft_lstsize(t_list *lst);
t_list			*ft_lstlast(t_list *lst);
t_list			*ft_lst_struct_new(void);
int				ft_strcmp(char *s1, char *s2);
char			*ft_strdup(const char *s);
char			*ft_strjoin(char const *s1, char const *s2);
char			*ft_free(char **tab, size_t i);
char			*from_lst_to_str(t_list *a);
void			from_lst_a_to_lst_b(t_list **a, t_list **b);
void			print_lst(t_list *lst);
void			print_lst_after_parse(t_list *lst);
void			init_states(t_state *state);
int				s_quoted_word(char *line, t_list **lst, int i);
int				d_quoted_word(char *line, t_tokens *tokens, int i, char **envp);
void			ft_scan_line(char *line, t_tokens *tokens, char **envp);
int				whats_the_state(char *line, t_tokens *tokens, int i, char **envp);
int				there_is_env_var(char *line, int i, t_tokens *tokens, char **envp);
int				get_env_var_value(t_list **item, t_list **start, char **envp);
int				there_is_redir(char *line, int i, t_tokens *tokens);
t_list			*remove_a_list_item(t_list **lst, t_list *item);

t_list			*retoken_env_var(char *env_var);
void			get_redirections_list(t_tokens *tokens, t_list **parse);
void			get_exec_list(t_tokens *tokens, t_list **parse);
int				check_fd_redir(char *word);
void			free_parse_things(t_list *parse);
int				is_it_a_builtin(t_list *parse);

#endif