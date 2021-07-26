#include "../../minishell.h"

void    unset_all(const char *name, t_shell *shell)
{
    long  i;
    long  j;
    long  k;
    char    **new_env;

    j = 0;
    i = find_env_all_var(name, shell);
    if (i == -1)
    {
        shell->cmd_status = SUCESS;
        return ;
    }
    while (shell->env_all[j++]);
    new_env = malloc(sizeof(char *) * (j - 1));
    j = 0;
    k = 0;
    while (shell->env_all[j])
    {
        if (j != i)
            new_env[k++] = ft_strdup(shell->env_all[j]);
        j++;
    }
    new_env[k] = NULL;
    ft_freee(shell->env_all);
    shell->env_all = new_env;
}

void    do_unset(const char *name, t_shell *shell)
{
    long  i;
    long  j;
    long  k;
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
    ft_freee(shell->env);
    shell->env = new_env;
    unset_all(name, shell);
    shell->cmd_status = SUCESS;
}

void    unset(t_list *lst, t_shell *shell, t_fd fd)
{
    if (!lst->lst_struct->exec)
        return ;
    lst->lst_struct->exec = lst->lst_struct->exec->next;
    while (lst->lst_struct->exec)
    {
        do_unset(lst->lst_struct->exec->content.word, shell);
        lst->lst_struct->exec = lst->lst_struct->exec->next;
    }
    fd = fd + 1;
    shell->cmd_status = SUCESS;
}