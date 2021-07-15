#include "minishell.h"

int	reading_word_state(char *line, int i, t_tokens *tokens, t_list *new)
{
	char	last;

	last = 0;
	if (line[i] == '$')
		i = there_is_env_var(line, ++i, &tokens->temp, &tokens->words);
	else
	{
		new = ft_lstnew(NULL, line[i]);
		if (line[i] == '|' || line[i] == '<' || line[i] == '>')
		{
			if (tokens->temp)
				from_lst_a_to_lst_b(&tokens->temp, &tokens->words);
			new->flag = SPECIAL;
		}
		else
		{
			if (tokens->temp)
				last = ft_lstlast(tokens->temp)->content.c;
			if (last == '|' || last == '<' || last == '>')
				from_lst_a_to_lst_b(&tokens->temp, &tokens->words);
			new->flag = NONE;
		}
		ft_lstadd_back(&tokens->temp, new);
	}
	return (i);
}

int	whats_the_state(char *line, t_tokens *tokens, int i)
{
	t_list	*new;

	new = NULL;
	if (tokens->state.s_quoted_word)
		i = s_quoted_word(line, &tokens->temp, ++i);
	else if (tokens->state.d_quoted_word)
		i = d_quoted_word(line, &tokens->temp, &tokens->words, ++i);
	else if (tokens->state.reading_word == 0)
	{
		if (tokens->temp)
			from_lst_a_to_lst_b(&tokens->temp, &tokens->words);
		init_states(&tokens->state);
	}
	else if (tokens->state.reading_word)
		i = reading_word_state(line, i, tokens, new);
	return (i);
}

int	there_is_space(char *line, int i, t_tokens *tokens)
{
	t_list	*new;

	if (tokens->temp)
		from_lst_a_to_lst_b(&tokens->temp, &tokens->words);
	new = ft_lstnew(NULL, line[i]);
	new->flag = SPACE;
	ft_lstadd_back(&tokens->temp, new);
	while (line[i] && line[i] == ' ' && line[i + 1] == ' ')
		i++;
	if (tokens->temp)
		from_lst_a_to_lst_b(&tokens->temp, &tokens->words);
	return (i);
}

void	ft_scan_line(char *line, t_tokens *tokens)
{
	int	i;

	i = 0;
	while (line[i])
	{
		init_states(&tokens->state);
		if (line[i] == '\"')
			tokens->state.d_quoted_word = 1;
		else if (line[i] == '\'')
			tokens->state.s_quoted_word = 1;
		else if (line[i] == '$')
			tokens->state.dollar = 1;
		if (line[i] == ' ')
			i = there_is_space(line, i, tokens);
		else
			i = whats_the_state(line, tokens, i);
		if (line[i] && !(tokens->words && !tokens->temp
				&& ft_lstlast(tokens->words)->flag == DOLLAR))
			i++;
	}
	if (tokens->temp)
		from_lst_a_to_lst_b(&tokens->temp, &tokens->words);
	// if (tokens->words)
	// {
	// 	printf("-----------------\n");
	// 	print_lst(tokens->words);
	// }
}
