#include "minishell.h"

void	free_parse_things(t_list *parse)
{
	t_list	*first;

	first = parse;
	while (parse)
	{
		if (parse->lst_struct->exec)
			ft_lstclear(&parse->lst_struct->exec);
		if (parse->lst_struct->redir)
			ft_lstclear(&parse->lst_struct->redir);
		if (parse->lst_struct)
		{
			free(parse->lst_struct);
			parse->lst_struct = NULL;
		}
		parse = parse->next;
	}
	parse = first;
	ft_lstclear(&parse);
}