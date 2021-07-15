#include "minishell.h"

char	**builtins_names_tab()
{
	char **tab;

	tab = malloc(7 * sizeof(char*));
	if (tab == NULL)
		return (NULL);
	tab[0] = ft_strdup("echo");
	if (tab[0] == NULL)
		return (NULL);
	tab[1] = ft_strdup("cd");;
	if (tab[1] == NULL)
		return (NULL);
	tab[2] = ft_strdup("pwd");;
	if (tab[2] == NULL)
		return (NULL);
	tab[3] = ft_strdup("export");;
	if (tab[3] == NULL)
		return (NULL);
	tab[4] = ft_strdup("unset");;
	if (tab[4] == NULL)
		return (NULL);
	tab[5] = ft_strdup("env");;
	if (tab[5] == NULL)
		return (NULL);
	tab[6] = ft_strdup("exit");;
	if (tab[6] == NULL)
		return (NULL);
	return (tab);
}

int	is_it_a_builtin(t_list *parse)
{
	char	**tab;
	char	*name;
	int		i;
	int		result;

	i = 0;
	result = 1;
	tab = builtins_names_tab();
	if (!parse->lst_struct->exec)
		return (-1);
	name = parse->lst_struct->exec->content.word;
	while (i < 7 && result != 0)
	{
		result = ft_strcmp(name, tab[i]);
		if (result != 0)
			i++;
	}
	ft_free(tab, 7);
	if (result != 0)
		return (-1);
	else
		return (i);
}
