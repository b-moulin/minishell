#include "../../minishell.h"

void    pwd(t_fd    output, t_shell *shell)
{
    char info[255];

    if (getcwd(info, 255) == NULL) {
        shell->cmd_status = 1;
    }
    write(output, info, str_len(info));
    shell->cmd_status = SUCESS;
}