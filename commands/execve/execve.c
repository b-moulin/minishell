#include "../../minishell.h"
#include <string.h>
#include <assert.h>
#include <errno.h>
// COMMENT CREER LE FICHIER AVANT D'EXECUTER EXECVE
// fd = open("test", O_CREAT | O_TRUNC | O_WRONLY, 0600); 

void    do_waitpid(pid_t  pid, int *i)
{

    // printf("------ pid %d\n", pid);
    waitpid(pid, i, 0);

    // if (WIFEXITED(exit_status))
    // {
    // dprintf(1, "<%i>\n", *i);
    int exit_status1 = WEXITSTATUS(*i);        
    // printf("exit_status %d\n", exit_status1);
        // printf("Exit status of the child was %d\n", exit_status);
    // }
    // WIFEXITED(exit_status);

    // printf("exit_status %d %d\n", exit_status, WIFEXITED(exit_status));
    // printf("waitpid %d %d\n", w, pid);

    // if (WIFEXITED(exit_status))
        // printf("terminé, code=%d\n", WEXITSTATUS(exit_status));
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
    // printf("pid_fork %d\n", pid);
    if (pid == -1)
        return ;
    exve = pid;
    if (exve == 0)
    {
        envarg = get_env_arg(shell->env, "PATH");
        path = ft_split(envarg, ':');
        while (path[i])
        {
            // printf("here\n");
            if (fd != 1)
                dup2(fd, 1);
            all_path = ft_strjoin(path[i], "/");
            all_path = ft_strjoin(all_path, command);
            exve = execve(all_path, argv, shell->env);
            // printf("pid : %d\n", pid);
            i++;
        }
        // else
        //     shell->cmd_status = SUCESS;
        if (fd != 1)
            close(fd);
    }
    if (exve == -1)
    {
        printf("bash: %s: command not found\n", command);
        shell->cmd_status = FAILED;
        wait(NULL);
        return ;
    }
    do_waitpid(pid, &exit_status);

    // puts(strerror(errno));
    // printf("errno %d\n", errno);
    wait(NULL);
}
