#include "minishell.h"

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

int     main(int argc, char **argv, char **envp)
{
    t_shell     *shell;

    shell = malloc(sizeof(t_shell));
    init_env(envp, shell);
    // printf("env %d\n", find_env_var("HOSTTYPE", shell));
    // printf("%s\n", get_env_arg(shell->env, "HOME"));
    // pwd(1, shell);
    // export("TOTO", "bmoulin", shell);
    // env(1, shell);
    // printf("\n\n\n");
    // unset("TOTO", shell);
    // env(1, shell);
    char *ttt[] = { "-a", NULL };
    do_execve(shell, "ls", ttt);
    env(1, shell);
}