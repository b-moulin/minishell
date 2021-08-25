#include "minishell.h"

void	free_parse_things(t_list *parse)
{
	t_list	*first;

	first = parse;
	while (parse)
	{
		puts("BBBBBBBBBBBBBBB");
		if (parse->lst_struct->exec)
		{
			puts("CCCCCCCCCCCCC");
		}
		ft_lstclear(&parse->lst_struct->exec);
		if (parse->lst_struct->redir)
		{
			puts("DDDDDDDDDDDDD");
		}
		ft_lstclear(&parse->lst_struct->redir);
		if (parse->lst_struct)
		{
			puts("AAAAAAAAAAAAAA");
			free(parse->lst_struct);
			parse->lst_struct = NULL;
		}
		parse = parse->next;
	}
	parse = first;
	ft_lstclear(&parse);
}

void	free_tokens_things(t_tokens *tokens, int error)
{
	if (tokens->temp)
		ft_lstclear(&tokens->temp);
	if (tokens->words)
		ft_lstclear(&tokens->words);
	if (error)
	{
		printf("Error\n");
		exit(0);
	}
}