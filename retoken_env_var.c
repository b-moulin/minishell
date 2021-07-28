#include "minishell.h"

t_list	*retoken_env_var(char *env_var)
{
	int		i;
	t_list	*new;
	t_list	*temp;
	t_list	*result;

	i = 0;
	new = NULL;
	temp = NULL;
	result = NULL;
	while (env_var[i])
	{
		while (env_var[i] && env_var[i] != ' ')
		{
			new = ft_lstnew(NULL, env_var[i]);
			new->flag = NONE;
			ft_lstadd_back(&temp, new);
			i++;
		}
		from_lst_a_to_lst_b(&temp, &result);
		if (env_var[i] == ' ')
		{
			new = ft_lstnew(NULL, env_var[i]);
			new->flag = SPACEE;
			ft_lstadd_back(&temp, new);
			from_lst_a_to_lst_b(&temp, &result);
		}
		if (env_var[i])
			i++;
	}
	return (result);
}
