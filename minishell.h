# ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef int t_fd;

typedef enum e_bool
{
	FALSE,
    TRUE,
}	t_bool;

typedef enum e_status
{
	SUCESS,
    FAILED = 127,
}	t_status;

typedef struct s_shell
{
    char    **env;
    char    **env_all;
    char    *history[500];
    t_status  cmd_status;
}   t_shell;

void    print_history(t_shell   *shell);
int     tab_size(char   **tab);
int     find_env_all_var(const char *tofind, t_shell *shell);
void    do_execve(t_shell   *shell, const char *command, char **argv);
int     get_next_line(char **line);
char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
size_t  str_len(const char *str);
char	**ft_split(char const *s, char c);

size_t  str_len(const char *str);
char    *get_env_arg(char **env, char *tofind);
int     find_env_var(const char *tofind, t_shell *shell);
void    ft_free(char    **tofree);
char	**ft_split(char const *s, char c);
char	*ft_substr(char const *s, unsigned int start, size_t len);

void    echo(const char *str, t_fd output, t_bool newline, t_shell  *shell);
void    cd(char *path, t_shell  *shell, t_fd fd);
void    pwd(t_fd    output, t_shell *shell);
void    init_env(char  **env, t_shell  *shell);
void    env(t_fd    output, t_shell *shell);
void    export(const char *name, const char *arg, t_shell *shell, t_fd fd);
void    unset(const char *name, t_shell *shell);
void    exit_cmd(void);

#endif