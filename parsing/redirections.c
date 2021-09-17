/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efarin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 14:24:40 by efarin            #+#    #+#             */
/*   Updated: 2021/08/26 14:24:42 by efarin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	there_is_redir(char *line, int i, t_tokens *tokens)
{
	t_list	*new;
	int		k;

	k = 0;
	if (line[i + 1] == '>' || line[i + 1] == '<')
	{
		new = ft_lstnew(NULL, line[i + 1]);
		if (!new)
			free_tokens_things(tokens, 1);
		ft_lstadd_back(&tokens->temp, new);
		k = 1;
	}
	if (tokens->temp)
		if (!from_lst_a_to_lst_b(&tokens->temp, &tokens->words))
			free_tokens_things(tokens, 1);
	return (i + k);
}

int	check_fd_redir(char *word)
{
	int		i;

	i = 0;
	if (!word)
		return (0);
	while (word[i] && word[i] >= 48 && word[i] <= 57)
		i++;
	if (!word[i])
		return (1);
	return (0);
}
