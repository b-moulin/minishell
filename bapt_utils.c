#include "minishell.h"

char    *get_env_arg(char **env, char *tofind)
{
    size_t  i;
    char    *findarg;
    char    *newstr;
    char    *ret;

    i = 0;
    newstr = ft_strjoin(tofind, "=");
    while (env[i])
    {
        findarg = ft_strnstr(env[i], newstr, str_len(newstr));
        if (findarg)
        {
            ret = ft_strdup(findarg+str_len(newstr));
            free(newstr);
            return (ret);
        }
        i++;
    }
    free(newstr);
    return (NULL);
}

void    ft_freee(char    **tofree)
{
    size_t  i;

    i = 0;
    while (tofree[i])
    {
        free(tofree[i]);
        i++;
    }
    free(tofree);
}

int     tab_size(char   **tab)
{
    size_t  i;

    i = 0;
    while (tab[i++]);
    return (i - 1);
}