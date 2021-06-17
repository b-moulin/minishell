#include "../../minishell.h"

void    env(t_fd    output, t_shell *shell)
{
    size_t  i;

    i = 0;
    while (shell->env_all[i])
    {
        write(output, shell->env_all[i], str_len(shell->env_all[i]));
        write(output, "\n", 1);
        shell->cmd_status = SUCESS;
        i++;
    }
}
