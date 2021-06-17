#include "../../minishell.h"

void    do_execve(t_shell   *shell, const char *command, char **argv) // fd a gerer par la fonction qui gère les pipes
{
    char    **path;
    char    *envarg;
    char    *all_path;
    size_t  i;
    int     pid;

    i = 0;
    pid = fork();
    if (pid == -1)
        return ;
    if (pid == 0)
    {
        envarg = get_env_arg(shell->env, "PATH");
        path = ft_split(envarg, ':');
        while (path[i])
        {
            all_path = ft_strjoin(path[i], "/");
            all_path = ft_strjoin(all_path, command);
            pid = execve(all_path, argv, shell->env);
            i++;
        }
    }
    if (pid == -1)
    {
        shell->cmd_status = FAILED;
    }
    else
        shell->cmd_status = SUCESS;
    wait(NULL);
}