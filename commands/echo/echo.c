#include "../../minishell.h"

void    echo(const char *str, t_fd output, t_bool newline, t_shell  *shell)
{
    write(output, str, str_len(str));
    if (newline)
        write(output, "\n", 1);
    shell->cmd_status = SUCESS;
}