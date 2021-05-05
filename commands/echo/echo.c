#include "../../minishell.h"

void    echo(const char *str, t_fd output, t_bool newline, t_shell  *shell)
{
    if (write(output, str, str_len(str)) != -1)
    {
        shell->cmd_status = SUCESS;
    }
    else
        shell->cmd_status = FAILED;
    if (newline)
    {
        if (write(output, "\n", 1) != -1)
        {
            shell->cmd_status = SUCESS;
        }
        else
            shell->cmd_status = FAILED;
    }
}