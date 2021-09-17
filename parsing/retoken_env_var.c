/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retoken_env_var.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efarin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 14:25:51 by efarin            #+#    #+#             */
/*   Updated: 2021/08/26 14:25:54 by efarin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	lstnew_or_die(char c, t_list **new, t_list **temp, int flag)
{
	*new = ft_lstnew(NULL, c);
	if (new == NULL)
	{
		ft_lstclear(temp);
		ft_lstclear(new);
		return (0);
	}
	(*new)->flag = flag;
	ft_lstadd_back(temp, *new);
	return (1);
}

void	*clear_and_return(t_list **result)
{
	ft_lstclear(result);
	return (NULL);
}

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
			if (!lstnew_or_die(env_var[i++], &new, &temp, NONE))
				return (clear_and_return(&result));
		if (!from_lst_a_to_lst_b(&temp, &result))
			return (clear_and_return(&result));
		if (env_var[i] == ' ')
			if (!lstnew_or_die(env_var[i], &new, &temp, SPACEE)
				|| !from_lst_a_to_lst_b(&temp, &result))
				return (clear_and_return(&result));
		if (env_var[i])
			i++;
	}
	return (result);
}
