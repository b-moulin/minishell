#include "minishell.h"

void	get_redirections_list(t_tokens *tokens, t_list **parse)
{
	t_list	*new;
	int		i;

	i = 1;
	new = NULL;
	while (tokens->words && tokens->words->flag == SPECIAL
		&& tokens->words->content.word[0] != '|')
	{
		i = 1;
		while (tokens->words && i > 0 && tokens->words->content.word[0] != '|')
		{
			if (tokens->words->flag != SPACEE)
			{
				new = ft_lstnew(tokens->words->content.word, '0');
				if (!new)
				{
					free_parse_things(*parse);
					free_tokens_things(tokens, 1);
				}
				ft_lstadd_back(&(*parse)->lst_struct->redir, new);
			}
			if (tokens->words->flag != SPECIAL && tokens->words->flag != SPACEE)
				i--;
			tokens->words = tokens->words->next;
		}
	}
}

void	from_token_to_parse(t_tokens *tokens, t_list *parse, t_list *new)
{
	while (tokens->words && tokens->words->content.word[0] != '|')
	{
		if (tokens->words->flag == NONE || tokens->words->flag == DOLLAR)
		{
			if (tokens->words->next && tokens->words->next->flag == SPECIAL
				&& check_fd_redir(tokens->words->content.word))
				get_redirections_list(tokens, &parse);
			else
			{
				new = ft_lstnew(tokens->words->content.word, '0');
				if (!new)
				{
					free_parse_things(parse);
					free_tokens_things(tokens, 1);
				}
				ft_lstadd_back(&parse->lst_struct->exec, new);
			}
		}
		else if (tokens->words->flag == SPECIAL)
		{
			if (tokens->words->content.word[0] == '>'
				|| tokens->words->content.word[0] == '<')
				get_redirections_list(tokens, &parse);
		}
		if (tokens->words && tokens->words->content.word[0] != '|')
			tokens->words = tokens->words->next;
	}
}

void	get_exec_list(t_tokens *tokens, t_list **parse)
{
	t_list	*new;
	t_list	*parse_2;

	parse_2 = *parse;
	while (tokens->words)
	{
		new = ft_lst_struct_new();
		if (!new)
		{
			free_parse_things(*parse);
			free_tokens_things(tokens, 1);
		}
		ft_lstadd_back(parse, new);
		parse_2 = ft_lstlast(*parse);
		parse_2->lst_struct = malloc(sizeof(t_parse));
		parse_2->lst_struct->exec = NULL;
		parse_2->lst_struct->redir = NULL;
		if (!parse_2->lst_struct)
			return ;
		from_token_to_parse(tokens, parse_2, new);
		if (tokens->words)
			tokens->words = tokens->words->next;
	}
	// if (*parse)
	// 	print_lst_after_parse(*parse);
}
