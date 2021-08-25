#include "../../minishell.h"

void    echo(t_list *lst, t_shell *shell, t_fd fd)
{
    t_bool  newline;
    t_list  *exec;

    exec = lst->lst_struct->exec;
    newline = TRUE;
    if (!exec->next)
    {
        ft_putstr_fd("\n", fd);
        return ;
    }
    if (exec)
        exec = exec->next;
    if (exec->content.word && exec->content.word[0] == '-' && exec->content.word[1] && exec->content.word[1] == 'n' && exec->content.word[2] == 0)
        newline = FALSE;
    if (newline == FALSE)
        exec = exec->next;
    while (exec)
    {
        ft_putstr_fd(exec->content.word, fd);
        exec = exec->next;
        if (exec)
            write(fd, " ", 1);
    }
    if (newline)
        write(fd, "\n", 1);
    shell->cmd_status = SUCESS;
    if (fd != 1)
		close(fd);
}