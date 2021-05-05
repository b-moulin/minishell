#include "../../minishell.h"

void    pwd(t_fd    output, t_shell *shell)
{
    char info[255];
 
    if (getcwd(info, 255) == NULL) {
        shell->cmd_status = FAILED;
    }
    shell->cmd_status = SUCESS;
    if (write(output, info, str_len(info)) == -1)
    {
        shell->cmd_status = FAILED;
    }
    else
        shell->cmd_status = SUCESS;
}