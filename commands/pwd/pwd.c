#include "../../minishell.h"

void    pwd(t_list *lst, t_shell *shell, t_fd fd)
{
    char info[255];

    lst = lst->lst_struct->exec->next;
    if (getcwd(info, 255) == NULL)
    {
        shell->cmd_status = 1;
    }
    ft_putstr_fd(info, fd);
    shell->cmd_status = SUCESS;
}