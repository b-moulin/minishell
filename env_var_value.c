#include "minishell.h"

t_list	*remove_a_list_item(t_list **lst, t_list *item)
{
	t_list	*first;
	t_list	*removed;
	
	first = *lst;
	if (*lst == item)
	{
		first = (*lst)->next;
		free(*lst);
		*lst = NULL;
		*lst = first;
		if (*lst == NULL)			//réglé à la zeub, faut mieux faire
			exit(0);
		return (*lst);
	}
	while ((*lst)->next && (*lst)->next != item)
		*lst = (*lst)->next;
	removed = (*lst)->next;
	(*lst)->next = removed->next;
	free(removed);
	removed = NULL;
	*lst = first;
	return (*lst);
}

int	get_env_var_value(t_list *item, t_list *start)
{
	char	*env_var;

	if (item->flag == DOLLAR)
	{
		env_var = getenv(item->content.word);
		if (env_var == NULL)
		{
			item = remove_a_list_item(&start, item);
			return (0);
		}
		item->content.word = env_var;
	}
	item = item->next;
	print_lst(item);
	return (1);
}
