#include "../minishell.h"

void	wr_lstadd_back(t_wrlist **alst, t_wrlist *new)
{
	t_wrlist	*ok;

	if (*alst == NULL)
	{
		*alst = new;
		return ;
	}
	ok = *alst;
	while (ok->next != NULL)
		ok = ok->next;
	ok->next = new;
}

static t_wrlist	**wrgetter(void)
{
	static t_wrlist	*wrm;

	return (&wrm);
}

void	wrdestroy(void)
{
	t_wrlist	*current;
	t_wrlist	*next;
	t_wrlist	**wrstart;

	wrstart = wrgetter();
	current = *wrstart;
	while (current)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	*wrstart = 0;
}

void	*wrmalloc(unsigned long size)
{
	char	*buffer;
	t_wrlist	*new_elem;

	buffer = malloc(size);
	if (!(buffer))
	{
		wrdestroy();
		return (0);
	}
	new_elem = malloc(sizeof(t_wrlist));
	if (!(new_elem))
	{
		free(buffer);
		wrdestroy();
		return (0);
	}
	new_elem->content = buffer;
	new_elem->next = 0;
	wr_lstadd_back(wrgetter(), new_elem);
	return (buffer);
}

