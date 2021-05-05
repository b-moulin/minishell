# ifndef MINISHELL_H
#define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <dirent.h>

typedef int t_fd;

typedef enum e_bool
{
	FALSE,
    TRUE,
}	t_bool;

typedef enum e_status
{
	SUCESS,
    FAILED,
}	t_status;

typedef struct s_shell
{
    char    **env;
    t_status  cmd_status;
}   t_shell;

char	*ft_strdup(const char *s1);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strnstr(const char *haystack, const char *needle, size_t len);
size_t  str_len(const char *str);

size_t  str_len(const char *str);
char    *get_env_arg(char **env, char *tofind);

void    echo(const char *str, t_fd output, t_bool newline, t_shell  *shell);
void    cd(char **path, t_shell  *shell);
void    pwd(t_fd    output, t_shell *shell);
void    init_env(char  **env, t_shell  *shell);

#endif