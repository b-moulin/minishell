#include "minishell.h"

int     main(int argc, char **argv, char **envp)
{
    t_shell     *shell;

    shell = malloc(sizeof(t_shell));

    init_env(envp, shell);
    // printf("env %d\n", find_env_var("HOSTTYPE", shell));
    // printf("%s\n", get_env_arg(shell->env, "HOME"));
    // pwd(1, shell);
    export("TOTO", "bmoulin", shell);
    env(1, shell);
    printf("\n\n\n");
    unset("TOTO", shell);
    env(1, shell);
}