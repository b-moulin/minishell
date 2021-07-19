#include "../../minishell.h"

void    echo(t_list *lst, t_shell *shell, t_fd fd)
{
    t_bool  newline;

    if (lst->lst_struct->exec->content.word && lst->lst_struct->exec->content.word[0] == '-' && lst->lst_struct->exec->content.word[1] && lst->lst_struct->exec->content.word[1] == 'n' && lst->lst_struct->exec->content.word[2] == 0)
        newline = FALSE;
    lst->lst_struct->exec = lst->lst_struct->exec->next;
    while (lst->lst_struct->exec)
    {
        ft_putstr_fd(lst->lst_struct->exec->content.word, fd);
        lst->lst_struct->exec = lst->lst_struct->exec->next;
    }
    if (newline)
        write(fd, "\n", 1);
    shell->cmd_status = SUCESS;
}