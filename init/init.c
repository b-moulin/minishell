#include "../minishell.h"

char    *get_env_name(char  *env)
{
    size_t  i;

    i = 0;
    while (env[i] && env[i] != '=')
        i++;
    return (ft_substr(env, 0, i));
}

void    init_env_all(t_shell    *shell)
{
    int  i;
    size_t  j;
    char    *env_name;
    char    *env_arg;
    char    *tmp;
    char    *save;

    i = 0;
    j = 0;
    env_name = 0;
    tmp = 0;
    save = 0;
    env_arg = 0;
    while (shell->env[i])
        i++;
    shell->env_all = malloc(sizeof(char *) * (i + 1));
    i = 0;
    while (shell->env[i])
    {
        if (i == find_env_var("_", shell))
            i++;
        if (!shell->env[i])
            break ;
        env_name = get_env_name(shell->env[i]);
        env_arg = get_env_arg(shell->env, env_name);
        tmp = ft_strjoin("\"", env_arg);
        save = ft_strjoin("declare -x ", env_name);
        shell->env_all[j] = ft_strjoin(save, "=");
        free(save);
        save = ft_strjoin(shell->env_all[j], tmp);
        free(shell->env_all[j]);
        shell->env_all[j] = ft_strjoin(save, "\"");
        free(env_name);
        free(env_arg);
        free(tmp);
        free(save);
        j++;
        i++;
    }
    shell->env_all[j] = NULL;
}

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
    init_env_all(shell);
    shell->read_fd = -1;
}
