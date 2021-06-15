#include "../../minishell.h"

void    cd_tild_inpath(char *path, t_shell  *shell)
{
    char    *tmp;

    tmp = get_env_arg(shell->env, "HOME");
    (path)[0] = '/';
    path = ft_strjoin(tmp, path);
}

void    cd(char *path, t_shell  *shell, t_fd fd)
{
    struct dirent *lecture;
    DIR     *rep;
    char    *str;

    if (path[0] == '~')
        cd_tild_inpath(path, shell);
    rep = opendir(path);
    if (rep == NULL)
    {
        str = ft_strdup("bash: cd: ");
        str = ft_strjoin(str, path);
        str = ft_strjoin(str, ": No such file or directory\n");
        write(fd, str, str_len(str));
        shell->cmd_status = FAILED;
        return ;
    }
    closedir(rep);
    shell->cmd_status = SUCESS;
}
