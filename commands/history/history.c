#include "../../minishell.h"

void    print_history(t_shell   *shell, t_fd fd)
{
    int i;
    char    *str;

    i = 0;
    while (shell->history[i])
    {
        str = ft_strdup("    ");
        str = ft_strjoin(str, ft_itoa(i + 1));
        str = ft_strjoin(str, " ");
        str = ft_strjoin(str, shell->history[i++]);
        ft_putstr_fd(str, fd);
        ft_putstr_fd("\n", fd);
        free(str);
    }
}