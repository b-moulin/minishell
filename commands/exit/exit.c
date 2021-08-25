#include "../../minishell.h"

int check_exit_int(t_shell *shell, char *arg, t_fd fd)
{
    int i;

    i = 0;
    shell->cmd_status = shell->cmd_status + 0; // pour les flags avant de completer
    while (arg && arg[i])
    {
        if (arg[i] < '0' || arg[i] > '9')
        {
            ft_putstr_fd("exit\nbash: exit: ", fd);
            ft_putstr_fd(arg, fd);
            ft_putstr_fd(": numeric argument required\n", fd);
            return (-1);
        }
        i++;
    }
    return (1);
}

long long    check_exitcode(t_shell *shell, char *arg, t_fd fd)
{
    long long   ret_value;

    if (!arg)
        return (0);
    ret_value = ft_atoi(arg);
    shell->cmd_status = shell->cmd_status + 0; // pour les flags avant de completer
    if (ret_value == -1)
    {
        ft_putstr_fd("exit\nbash: exit: ", fd);
        ft_putstr_fd(arg, fd);
        ft_putstr_fd(": numeric argument required\n", fd);
        return (-1);
    }
    return (ret_value % 255);
}

void    exit_cmd(t_list *lst, t_shell *shell, t_fd fd)
{
    long long   ret_value;
    char        **args;
    t_list      *exec;

    exec = lst->lst_struct->exec;
    args = 0;
    if (exec)
        exec = exec->next;
    if (exec && exec->content.word && exec->next && exec->next->content.word)
    {
        args = malloc(sizeof(char *) * 3);
        if (!args)
            return ;
        args[2] = 0;
        args[0] = ft_strdup(exec->content.word);
        args[1] = ft_strdup(exec->next->content.word);
    }
    else if (exec && exec->content.word)
    {
        args = malloc(sizeof(char *) * 3);
        if (!args)
            return ;
        args[1] = 0;
        args[0] = ft_strdup(exec->content.word);
    }
    if (args && args[0] && args[1])
    {
        if (check_exit_int(shell, args[0], fd) == -1)
            exit(255);
        ft_putstr_fd("bash: exit: too many arguments\n", fd);
        shell->cmd_status = 1;
        return ;
    }
    if (args)
    {
        ret_value = check_exit_int(shell, args[0], fd);
    }
    else
    {
        ret_value = check_exit_int(shell, NULL, fd);
    }
    if (ret_value == -1)
    {
        exit(255);
    }
    if (args)
    {
        ret_value = check_exitcode(shell, args[0], fd);
    }
    else
    {
        ret_value = check_exitcode(shell, NULL, fd);
    }
    if (ret_value == -1)
        exit(255);
    exit(ret_value);
}
// 9223372036854775807