#include "../minishell.h"

void     init_env(char  **env, t_shell  *shell)
{
    size_t  i;

    i = 0;
    while (env[i])
        i++;
    i++;
    shell->env = malloc(sizeof(char *) * i);
    if (!shell->env)
        return ;
    i = 0;
    while (env[i])
    {
        shell->env[i] = ft_strdup(env[i]);
        i++;
    }
    shell->env[i] = NULL;
}
