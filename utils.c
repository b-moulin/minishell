#include "minishell.h"

char	*from_lst_to_str(t_list *a)
{
	t_list	*start;
	int		i;
	char	*str;
	int		len;

	i = -1;
	start = a;
	len = ft_lstsize(a);
	str = malloc((len + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	while (++i < len)
	{
		str[i] = a->content.c;
		a = a->next;
	}
	str[i] = '\0';
	a = start;
	return (str);
}

void	from_lst_a_to_lst_b(t_list **a, t_list **b)
{
	t_list	*new;
	char	*str;

	str = from_lst_to_str(*a);
	if (str == NULL)
		return ;
	new = ft_lstnew(str, 0);
	new->flag = (*a)->flag;
	ft_lstadd_back(b, new);
	ft_lstclear(a);
}

void	print_lst(t_list *lst)
{
	t_list	*newlst;

	newlst = lst;
	while (newlst && newlst->content.word)
	{
		printf("[%s]\n", newlst->content.word);
		printf("flag = %i\n", newlst->flag);
		newlst = newlst->next;
	}
}

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
