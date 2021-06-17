#include "../../minishell.h"

t_bool  is_valid_env_name(const char *env)
{
    size_t  i;

    i = 0;
    if ((env[i] >= 'a' && env[i] <= 'z') || (env[i] >= 'A' && env[i] <= 'Z') || env[i] == '_')
    {
        i++;
        while (env[i])
        {
            if(!((env[i] >= 'a' && env[i] <= 'z') || (env[i] >= 'A' && env[i] <= 'Z') || env[i] == '_' || (env[i] >= '0' && env[i] <= '9')))
                return (FALSE);
            i++;
        }
    }
    else
        return (FALSE);
    return (TRUE);
}

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

int    find_env_all_var(const char *tofind, t_shell *shell)
{
    size_t  i;
    char    *ret_strnstr;
    char    *tmp;

    i = 0;
    while (shell->env_all[i])
    {
        tmp = ft_strjoin("declare -x ", tofind);
        tmp = ft_strjoin(tmp, "=");
        ret_strnstr = ft_strnstr(shell->env_all[i], tmp, str_len(tmp));
        free(tmp);
        if (ret_strnstr)
            return (i);
        i++;
    }
    return (-1);
}

void    export_no_name(const char *name, const char *arg, t_fd fd, t_shell *shell)
{
    size_t  i;
    int  there_is_par;
    char    *tmp;
    char    *str;

    there_is_par = 0;

    i = 0;
    while (arg[i])
    {
        if (arg[i] == '(' || arg[i] == ')')
            there_is_par++;
        i++;
    }
    if (there_is_par >= 1)
    {
        write(fd, "bash: syntax error near unexpected token `='\n", str_len("bash: syntax error near unexpected token `='\n"));
        shell->cmd_status = FAILED;
        return ;
    }
    tmp = ft_strdup("bash: syntax error near unexpected token `");
    str = ft_strjoin(tmp, arg);
    tmp = ft_strjoin(str, "'\n");
    shell->cmd_status = FAILED;
    write(fd, tmp, str_len(tmp));
}

void    export_error(const char *name, const char *arg, t_fd fd, t_shell *shell)
{
    char    *tmp;
    char    *str;
    size_t  i;
    size_t  save;
    char    *token;
    size_t  count;

    i = 0;
    count = 0;
    token = 0;
    tmp = 0;
    save = 0;
    if (str_len(name) == 2 && name[0] == '(' && name[1] == ')')
    {
        export_no_name(name, arg, fd, shell);
        return ;
    }
    while (name[i])
    {
        if (name[i] == '(' || name[i] == ')')
        {
            count++;
            if ((count == 2 && name[0] == '(') || name[0] == ')')
            {
                tmp = ft_substr(name, save+1, i-save-1);
            }
            if (!token)
            {
                token = ft_strdup("0");
                token[0] = name[i];
                token[1] = 0;
            }
            save = i;
        }
        i++;
    }
    if (count > 0 && count % 2 == 0 && tmp)
    {
        str = ft_strjoin("bash: syntax error near unexpected token `", tmp);
        tmp = ft_strjoin(str, "'\n");
        write(fd, tmp, str_len(tmp));
        shell->cmd_status = FAILED;
        return ;
    }
    if (count > 0 && count % 2 == 1)
    {
        str = ft_strjoin("bash: syntax error near unexpected token `", token);
        tmp = ft_strjoin(str, "'\n");
        write(fd, tmp, str_len(tmp));
        shell->cmd_status = FAILED;
        return ;
    }
    if (token != 0)
    {
        str = ft_strjoin("bash: syntax error near unexpected token `", token);
        if (str_len(name) == 2 && name[0] == '(' && name[1] == ')')
        {
            tmp = ft_strjoin(str, arg);
            str = ft_strjoin(tmp, "'\n");
            write(fd, str, str_len(str));
            shell->cmd_status = FAILED;
            return ;
        }
        if (!arg || !arg[0])
        {
            tmp = ft_strjoin(str, "'\n");
            write(fd, tmp, str_len(tmp));
            shell->cmd_status = FAILED;
            return ;
        }
        tmp = ft_strjoin(str, "=");
        str = ft_strjoin(tmp, arg);
        tmp = ft_strjoin(str, "'\n");
        write(fd, tmp, str_len(tmp));
        shell->cmd_status = FAILED;
        return ;
    }
    tmp = ft_strjoin("bash: export: `", name);
    tmp = ft_strjoin(tmp, "\': not a valid identifier\n");
    write(fd, tmp, str_len(tmp));
    shell->cmd_status = FAILED;
}

void    export_add_arg(const char *name, const char *arg, t_shell *shell, t_fd fd)
{
    int     i;
    char    **new_env;
    char    *tmp;

    i = -1;
    if (!is_valid_env_name(name))
    {
        while (name[++i])
            if (name[i] == '(' || name[i] == ')')
                export_error(name, arg, fd, shell);
        tmp = ft_strdup("bash: export: `");
        tmp = ft_strjoin(tmp, name);
        tmp = ft_strjoin(tmp, "=");
        tmp = ft_strjoin(tmp, arg);
        tmp = ft_strjoin(tmp, "': not a valid identifier\n");
        write(fd, tmp, str_len(tmp));
        shell->cmd_status = FAILED;
        return ;
    }
    while (shell->env[++i]);
    new_env = malloc(sizeof(char *) * (i + 2));
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
    ft_free(shell->env);
    shell->env = new_env;
}

void    export_add_arg_all(const char *name, const char *arg, t_shell *shell)
{
    int     i;
    char    **new_env;
    char    *tmp;

    i = -1;
    while (shell->env_all[++i]);
    new_env = malloc(sizeof(char *) * (i + 2));
    if (!new_env)
        return ;
    i = 0;
    while (shell->env_all[i])
    {
        new_env[i] = ft_strdup(shell->env_all[i]);
        i++;
    }
    tmp = ft_strjoin("declare -x ", name);
    if (arg == NULL) // CMD example export TOTO
    {
        new_env[i] = tmp;
        new_env[i + 1] = NULL;
        ft_free(shell->env_all);
        shell->env_all = new_env;
        return ;
    }
    tmp = ft_strjoin(tmp, "=\"");
    tmp = ft_strjoin(tmp, arg);
    new_env[i] = ft_strjoin(tmp, "\"");
    free(tmp);
    new_env[i + 1] = NULL;
    ft_free(shell->env_all);
    shell->env_all = new_env;
}

void    export_all(const char *name, const char *arg, t_shell *shell)
{
    int     i;
    char    *tmp;

    i = find_env_all_var(name, shell);
    if (i == -1)
    {
        export_add_arg_all(name, arg, shell); // si name est pas déclarer
    }
    else
    {
        tmp = ft_strjoin("declare -x ", name);
        if (arg == NULL)
        {
            shell->env_all[i] = tmp;
            return ;
        }
        tmp = ft_strjoin(tmp, "=\"");
        tmp = ft_strjoin(tmp, arg);
        tmp = ft_strjoin(tmp, "\"");
        shell->env_all[i] = tmp;
    }
}

void    export(const char *name, const char *arg, t_shell *shell, t_fd fd)
{
    int  i;
    char    *tmp;

    i = find_env_var(name, shell);
    if (i == -1)
    {
        export_add_arg(name, arg, shell, fd);
    }
    else
    {
        tmp = ft_strjoin(name, "=");
        free(shell->env[i]);
        shell->env[i] = ft_strjoin(tmp, arg);
        free(tmp);
    }
    export_all(name, arg, shell);
    shell->cmd_status = SUCESS;
}
