#include "minishell.h"

int	retoken_env_var(t_list *words, t_list *item, char **envp)
{
	t_tokens	*dup;
	char		*env_var;
	t_list		*first;
	t_list		*next;

	dup = NULL;
	env_var = item->content.word;
	ft_scan_line(env_var, dup, envp);
	//!!enlever le dernier maillon
	first = dup->words;
	while (dup->words)
	{
		next = dup->words->next;
		ft_lstadd_back(&words, dup->words);
		dup->words = next;
	}
	dup->words = first;
	return (1);
}