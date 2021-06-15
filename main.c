#include "minishell.h"
#include <signal.h>

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
            execve(all_path, argv, shell->env);
            i++;
        }
    }
    wait(NULL);
}

/*

    EXEMPLES        FONCTIONS UTILISATIONS                  FONCTIONS PROTOTYPES

    echo    ==>     echo("Bonjour", fd, TRUE, shell);       echo(const char *str, t_fd output, t_bool newline, t_shell  *shell)
    cd      ==>     cd("DOSSIER", shell, fd);               cd(char *path, t_shell  *shell, t_fd fd)
    pwd     ==>     pwd(fd, shell);                         pwd(t_fd    output, t_shell *shell)
    export  ==>     export("TOTO", "bmoulin", shell, fd);   export(const char *name, const char *arg, t_shell *shell, t_fd fd)
    unset   ==>     unset("TOTO", shell);                   unset(const char *name, t_shell *shell)
    env     ==>     env(fd, shell);                         env(t_fd    output, t_shell *shell)
    exit    ==>     exit_cmd();                             exit_cmd(void)

*/

void Recuperation(int sig)
{
    char    c;

    c = 127;
    if (sig == SIGINT)
    {
        write(0, "\n", 1);
        write(0, "B", 1);
    }
}

int     main(int argc, char **argv, char **envp)
{
    t_shell     *shell;
    char        *cmd;
    char t[1]; //
    int i=0; //

    signal(SIGSEGV, Recuperation);
    signal(SIGINT,  Recuperation);
    cmd = NULL;
    shell = malloc(sizeof(t_shell));
    init_env(envp, shell);
    while (1)
    {
        write(1, "minishell# ", str_len("minishell# "));
        if (get_next_line(&cmd) == 0) // Ctrl-D ==> exit the shell
        {
            write(1, "\n", 1);
            exit(0);
        }
        printf("cmd |%s|\n", cmd);
    }
}
