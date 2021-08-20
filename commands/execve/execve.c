#include "../../minishell.h"
#include <string.h>
#include <assert.h>
#include <errno.h>
// COMMENT CREER LE FICHIER AVANT D'EXECUTER EXECVE
// fd = open("test", O_CREAT | O_TRUNC | O_WRONLY, 0600); 

void    do_waitpid(t_shell *shell, pid_t  pid, int *i)
{
    // if (!pid)
    // {
        waitpid(pid, i, 0);
        int exit_status1 = WEXITSTATUS(*i);   
        shell->cmd_status = exit_status1;
    // }
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
        if (fd != 1)
            dup2(fd, 1);
        while (path[i])
        {
            all_path = ft_strjoin(path[i], "/");
            all_path = ft_strjoin(all_path, command);
            exve = execve(all_path, argv, shell->env);
            i++;
        }
    }
    if (!pid)
        exit(0);
    if (exve == -1)
        exve = execve(command, argv, shell->env);
    if (exve == -1)
    {
        envarg = ft_strdup(command);
        ft_putstr_fd("bash: ", 2);
        ft_putstr_fd(envarg, 2);
        ft_putstr_fd(": command not found\n", 2);
        shell->cmd_status = FAILED;
        wait(NULL);
        exit(0);
        return ;
    }
    do_waitpid(shell, pid, &exit_status);
    // waitpid(pid, 1, NULL);
    // wait(NULL);
    if (fd > 1)
        close(fd);
}

void    doo_execve(t_list *lst, t_shell *shell, t_fd fd)
{
    int     i;
    t_list  *save;
    char    *cmd;
    char    **args;
    // int     pid;

    save = lst;
    cmd = ft_strdup(lst->lst_struct->exec->content.word);
    i = ft_lstsize(lst->lst_struct->exec);
    args = 0;
    args = malloc(sizeof(char *) * (i + 1));
    args[i] = 0;
    i = 0;
    while (lst->lst_struct->exec)
    {
        args[i] = ft_strdup(lst->lst_struct->exec->content.word);
        i++;
        lst->lst_struct->exec = lst->lst_struct->exec->next;
    }
    do_execve(shell, cmd, args, fd);
}