#include "../../minishell.h"

void    env(t_fd    output, t_shell *shell)
{
    size_t  i;

    i = 0;
    while (shell->env[i])
    {
        write(output, shell->env[i], str_len(shell->env[i]));
        write(output, "\n", 1);
        shell->cmd_status = SUCESS;
        i++;
    }
}
