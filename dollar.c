#include "minishell.h"

int	is_it_env_var_separator(char c)
{
	if ((c >= 32 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 94)
		|| c == 123 || (c >= 125 && c <= 127) || c == 96)
		return (1);
	else
		return (0);
}

int	there_is_env_var(char *line, int i, t_tokens *tokens, char **envp)
{
	t_list	*new;
	int		start;
	t_list	*item;

	start = i;
	item = NULL;
	new = NULL;
	if (tokens->temp)
		from_lst_a_to_lst_b(&tokens->temp, &tokens->words);
	while (line[i] && !is_it_env_var_separator(line[i]))
	{
		new = ft_lstnew(NULL, line[i]);
		new->flag = DOLLAR;
		ft_lstadd_back(&tokens->temp, new);
		i++;
	}
	if (i == start)
	{
		i--;
		new = ft_lstnew(NULL, line[i]);
		new->flag = NONE;
		ft_lstadd_back(&tokens->temp, new);
		i++;
	}
	from_lst_a_to_lst_b(&tokens->temp, &tokens->words);
	if (i != start)
	{
		item = ft_lstlast(tokens->words);
		get_env_var_value(item, tokens->words, envp);
	}
	return (i);
}
