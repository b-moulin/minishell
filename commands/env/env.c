#include "../../minishell.h"

void    env(t_fd    output, t_shell *shell)
{
    size_t  i;

    i = 0;
    while (shell->env[i])
    {
        if (write(output, shell->env[i], str_len(shell->env[i])) == -1 || write(output, "\n", 1) == -1)
        {
            shell->cmd_status = FAILED;
        }
        else
            shell->cmd_status = SUCESS;
        i++;
    }
}
