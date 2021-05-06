#include "../../minishell.h"

void    unset(const char *name, t_shell *shell)
{
    size_t  i;
    size_t  j;
    size_t  k;
    char    **new_env;

    j = 0;
    i = find_env_var(name, shell);
    if (i == -1)
    {
        shell->cmd_status = SUCESS;
        return ;
    }
    while (shell->env[j++]);
    new_env = malloc(sizeof(char *) * (j - 1));
    j = 0;
    k = 0;
    while (shell->env[j])
    {
        if (j != i)
            new_env[k++] = ft_strdup(shell->env[j]);
        j++;
    }
    new_env[k] = NULL;
    ft_free(shell->env);
    shell->env = new_env;
    shell->cmd_status = SUCESS;
}
