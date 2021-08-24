#include "minishell.h"

void	join_the_tokens(t_tokens *tokens, t_list **words)
{
	t_list	*new;
	t_list	*first;
	char	*str;

	new = NULL;
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
		ft_lstadd_back(&tokens->temp, new);
		if (new->flag == SPECIAL)
			i = there_is_redir(line, i, tokens);
	}
	return (i);
}

int	whats_the_state(char *line, t_tokens *tokens, int i, t_shell *exec_part)
{
	if (tokens->state.s_quoted_word)
	{
		i = s_quoted_word(line, &tokens->temp, ++i);
		if (i < 0)
			free_tokens_things(tokens, 1);
	}
	else if (tokens->state.d_quoted_word)
		i = d_quoted_word(line, tokens, ++i, exec_part);
	else if (tokens->state.reading_word == 0)
	{
		if (tokens->temp)
			if (!from_lst_a_to_lst_b(&tokens->temp, &tokens->words))
				free_tokens_things(tokens, 1);
		init_states(&tokens->state);
	}
	else if (tokens->state.reading_word)
		i = reading_word_state(line, i, tokens, exec_part);
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
