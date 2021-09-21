/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efarin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:48:46 by efarin            #+#    #+#             */
/*   Updated: 2021/09/17 17:48:49 by efarin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	join_the_tokens(t_tokens *tokens, t_list **words)
{
	t_list	*first;
	char	*str;

	first = *words;
	str = NULL;
	while (*words)
	{
		if ((*words)->next && (*words)->next->flag == (*words)->flag)
		{
			str = ft_strjoin((*words)->content.word,
					(*words)->next->content.word);
			if (!str)
				free_tokens_things(tokens, 1);
			free((*words)->content.word);
			(*words)->content.word = str;
			remove_a_list_item(words, (*words)->next);
		}
		else
			*words = (*words)->next;
	}
	*words = first;
}

void	special_token(t_list *new, t_tokens *tokens)
{
	if (tokens->temp)
		if (!from_lst_a_to_lst_b(&tokens->temp, &tokens->words))
			free_tokens_things(tokens, 1);
	new->flag = SPECIAL;
}

void	none_flag_token(char last, t_tokens *tokens, t_list *new)
{
	if (tokens->temp)
		last = ft_lstlast(tokens->temp)->content.c;
	if (last == '|' || last == '<' || last == '>')
	{
		if (!from_lst_a_to_lst_b(&tokens->temp, &tokens->words))
		{
			free(new);
			free_tokens_things(tokens, 1);
		}
	}
	new->flag = NONE;
}

int	reading_word_state(char *line, int i, t_tokens *tokens, t_shell *exec_part)
{
	char	last;
	t_list	*new;

	last = 0;
	new = NULL;
	if (line[i] == '$')
		i = there_is_env_var(line, ++i, tokens, exec_part);
	else
	{
		new = ft_lstnew(NULL, line[i]);
		if (!new)
			free_tokens_things(tokens, 1);
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
			special_token(new, tokens);
		else
			none_flag_token(last, tokens, new);
		ft_lstadd_back(&tokens->temp, new);
		if (new->flag == SPECIAL)
			i = there_is_redir(line, i, tokens);
	}
	return (i);
}

int	there_is_space(char *line, int i, t_tokens *tokens)
{
	t_list	*new;

	if (tokens->temp)
		if (!from_lst_a_to_lst_b(&tokens->temp, &tokens->words))
			free_tokens_things(tokens, 1);
	new = ft_lstnew(NULL, line[i]);
	if (!new)
		free_tokens_things(tokens, 1);
	new->flag = SPACEE;
	ft_lstadd_back(&tokens->temp, new);
	while (line[i] && line[i] == ' ' && line[i + 1] == ' ')
		i++;
	if (tokens->temp)
		if (!from_lst_a_to_lst_b(&tokens->temp, &tokens->words))
			free_tokens_things(tokens, 1);
	return (i);
}
