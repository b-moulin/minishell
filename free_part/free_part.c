#include "../minishell.h"

void    free_double_tab(char **tab)
{
    int i;

    i = 0;
    if (!tab)
        return ;
    while (tab[i])
    {
        free(tab[i]);
        i++;
    }
    free(tab);
}

void    free_all_env(t_shell *shell)
{
    // printf("ici !\n");
    free_double_tab(shell->env);
    free_double_tab(shell->env_all);
}