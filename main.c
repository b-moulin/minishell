#include "minishell.h"

int     main(int argc, char **argv, char **env)
{
    t_shell     *shell;


    shell = malloc(sizeof(t_shell));

    init_env(env, shell);
    // printf("%s\n", get_env_arg(shell->env, "HOME"));
    pwd(1, shell);
}