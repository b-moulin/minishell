#include "../../minishell.h"
#include <string.h>
#include <assert.h>
#include <errno.h>
// COMMENT CREER LE FICHIER AVANT D'EXECUTER EXECVE
// fd = open("test", O_CREAT | O_TRUNC | O_WRONLY, 0600); 

void    do_waitpid(t_shell *shell, pid_t  pid, int *i)
{
    waitpid(pid, i, 0);
    int exit_status1 = WEXITSTATUS(*i);   
    shell->cmd_status = exit_status1;
}

void    do_execve(t_shell   *shell, const char *command, char **argv, int fd) // fd a gerer par la fonction qui gère les pipes
{
    char    **path;
    char    *envarg;
    char    *all_path;
    size_t  i;
    int     pid;
    int     exve;
    int     exit_status = 0;

    i = 0;
    pid = fork();
    if (pid == -1)
        return ;
    exve = pid;
    if (exve == 0)
    {
        envarg = get_env_arg(shell->env, "PATH");
        path = ft_split(envarg, ':');
        while (path[i])
        {
            if (fd != 1)
                dup2(fd, 1);
            all_path = ft_strjoin(path[i], "/");
            all_path = ft_strjoin(all_path, command);
            exve = execve(all_path, argv, shell->env);
            i++;
        }
        if (fd != 1)
            close(fd);
    }
    if (exve == -1)
    {
        envarg = ft_strdup(command);
        ft_putstr_fd("bash: ", 2);
        ft_putstr_fd(envarg, 2);
        ft_putstr_fd(": command not found\n", 2);
        shell->cmd_status = FAILED;
        wait(NULL);
        return ;
    }
    do_waitpid(shell, pid, &exit_status);
    wait(NULL);
}
