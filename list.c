#include "minishell.h"

t_list	*ft_lstnew(char *str, char c)
{
	t_list	*list;

	list = malloc(sizeof(t_list));
	if (!(list))
		return (NULL);
	if (str == NULL)
	{
		list->content.c = c;
		list->content.word = NULL;
	}
	else
	{
		list->content.c = '0';
		list->content.word = str;
	}
	list->next = NULL;
	return (list);
}

t_list	*ft_lst_struct_new(void)
{
	t_list	*liste;

	liste = malloc(sizeof(t_list));
	if (!(liste))
		return (NULL);
	liste->content.c = '0';
	liste->next = NULL;
	return (liste);
}

t_list	*ft_lstlast(t_list *lst)
{
	if (lst == NULL)
		return (lst);
	while (lst && lst->next != NULL)
		lst = lst->next;
	return (lst);
}

void	ft_lstadd_back(t_list **alst, t_list *new)
{
	t_list		*tmp;

	if (!*alst)
		*alst = new;
	else
	{
		tmp = ft_lstlast(*alst);
		tmp->next = new;
		tmp->next->next = NULL;
	}
}

void	ft_lstclear(t_list **lst)
{
	t_list	*next;
	t_list	*tmp;

	next = *lst;
	while (next != NULL)
	{
		tmp = next;
		next = next->next;
		free(tmp);
	}
	(*lst) = NULL;
}
