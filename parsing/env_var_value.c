/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_value.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efarin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 14:26:18 by efarin            #+#    #+#             */
/*   Updated: 2021/08/26 14:26:20 by efarin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

int	get_env_var_value(t_list **item, t_list **start, char **envp)
{
	char	*env_var;
	t_list	*res;

	env_var = NULL;
	env_var = get_env_arg(envp, (*item)->content.word);
	if (env_var == NULL)
	{
		*item = remove_a_list_item(start, *item);
		return (0);
	}
	(*item)->content.word = env_var;
	res = retoken_env_var(env_var);
	if (!res)
		return (0);
	(*item)->content.word = res->content.word;
	(*item)->flag = res->flag;
	(*item)->next = res->next;
	return (1);
}
