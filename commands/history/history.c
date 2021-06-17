#include "../../minishell.h"

void    print_history(t_shell   *shell)
{
    int i;

    i = 0;
    while (shell->history[i])
    {
        printf("    %d ", i + 1);
        printf("%s\n", shell->history[i++]);
    }
}