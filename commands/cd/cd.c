#include "../../minishell.h"

void    cd_tild_inpath(char **path, t_shell  *shell)
{
    char    *tmp;

    tmp = get_env_arg(shell->env, "HOME");
    (*path)[0] = '/';
    *path = ft_strjoin(tmp, *path);
}

void    cd(char **path, t_shell  *shell)
{
    struct dirent *lecture;
    DIR *rep;
    if ((*path)[0] == '~')
        cd_tild_inpath(path, shell);
    rep = opendir(*path);
    if (!rep)
    {
        shell->cmd_status = FAILED;
        return ;
    }
    // while ((lecture = readdir(rep)))
    // {
    //    printf("FICHIER: %s\n", lecture->d_name);
    // }
    closedir(rep);
    shell->cmd_status = SUCESS;
}
