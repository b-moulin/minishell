#include "../../minishell.h"

int    find_env_var(const char *tofind, t_shell *shell)
{
    size_t  i;
    char    *ret_strnstr;
    char    *tmp;

    i = 0;
    while (shell->env[i])
    {
        tmp = ft_strjoin(tofind, "=");
        ret_strnstr = ft_strnstr(shell->env[i], tmp, str_len(tmp));
        free(tmp);
        if (ret_strnstr)
            return (i);
        i++;
    }
    return (-1);
}

void    export_add_arg(const char *name, const char *arg, t_shell *shell)
{
    size_t  i;
    char    **new_env;
    char    *tmp;

    i = -1;
    while (shell->env[++i]);
    new_env = malloc(sizeof(char *) * (i + 1));
    if (!new_env)
        return ;
    i = 0;
    while (shell->env[i])
    {
        new_env[i] = ft_strdup(shell->env[i]);
        i++;
    }
    tmp = ft_strjoin(name, "=");
    new_env[i] = ft_strjoin(tmp, arg);
    free(tmp);
    new_env[i + 1] = NULL;
    i = 0;
    while (shell->env[i])
    {
        free(shell->env[i]);
        i++;
    }
    free(shell->env);
    shell->env = new_env;
}

void    export(const char *name, const char *arg, t_shell *shell)
{
    size_t  i;
    char    *tmp;

    i = find_env_var(name, shell);
    if (i == -1)
    {
        export_add_arg(name, arg, shell);
    }
    else
    {
        printf("find\n");
        tmp = ft_strjoin(name, "=");
        free(shell->env[i]);
        shell->env[i] = ft_strjoin(tmp, arg);
        free(tmp);
    }
    shell->cmd_status = SUCESS;
}
