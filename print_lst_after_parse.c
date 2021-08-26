#include "minishell.h"

void	print_lst_after_parse(t_list *lst)
{
	t_list	*new;
	t_list	*first_exec;
	t_list	*first_redir;

	new = lst;
	printf("------exec------\n");
	while (new)
	{
		printf("------\n");
		first_exec = new->lst_struct->exec;
		while (new->lst_struct->exec)
		{
			printf("[%s]\n", new->lst_struct->exec->content.word);
			new->lst_struct->exec = new->lst_struct->exec->next;
		}
		new->lst_struct->exec = first_exec;
		new = new->next;
	}
	new = lst;
	printf("------redir------\n");
	while (new)
	{
		printf("------\n");
		first_redir = new->lst_struct->redir;
		while (new->lst_struct->redir)
		{
			printf("[%s]\n", new->lst_struct->redir->content.word);
			new->lst_struct->redir = new->lst_struct->redir->next;
		}
		new->lst_struct->redir = first_redir;
		new = new->next;
	}
	new = lst;
}
