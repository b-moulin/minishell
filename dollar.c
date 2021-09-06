/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   dollar.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aviscogl <aviscogl@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2021/08/26 14:26:06 by efarin       #+#   ##    ##    #+#       */
/*   Updated: 2021/09/02 22:50:44 by aviscogl    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */
#include "minishell.h"

int	is_it_env_var_separator(char c)
{
	if ((c >= 32 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 94)
		|| c == 123 || c >= 125 || c == 96)
		return (1);
	else
		return (0);
}

void	add_cmd_state(t_tokens *tokens, t_shell *exec_part)
{
	int		i;
	t_list	*new;
	char	*state;

	i = 0;
	state = ft_itoa(exec_part->cmd_status);
	if (state == NULL)
		return ;
	while (state[i])
	{
		new = ft_lstnew(NULL, state[i]);
		if (!new)
		{
			free(state);
			free_tokens_things(tokens, 1);
		}
		new->flag = NONE;
		ft_lstadd_back(&tokens->temp, new);
		i++;
	}
}

int	no_env_var(int i, char *line, t_tokens *tokens, t_shell *exec_part)
{
	t_list	*new;

	new = NULL;
	if (line[i] && line[i] == '?')
		add_cmd_state(tokens, exec_part);
	else
	{
		i--;
		new = ft_lstnew(NULL, line[i]);
		if (!new)
			free_tokens_things(tokens, 1);
		new->flag = NONE;
		ft_lstadd_back(&tokens->temp, new);
		i++;
	}
	return (i);
}

void	manage_env_var(t_tokens *tokens, t_shell *exec_part)
{
	t_list	*item;

	item = ft_lstlast(tokens->words);
	item->j = 1;
	get_env_var_value(&item, &tokens->words, exec_part->env);
}

int	there_is_env_var(char *line, int i, t_tokens *tokens, t_shell *exec_part)
{
	t_list	*new;
	int		start;

	start = i;
	new = NULL;
	if (tokens->temp)
		if (!from_lst_a_to_lst_b(&tokens->temp, &tokens->words))
			free_tokens_things(tokens, 1);
	while (line[i] && !is_it_env_var_separator(line[i]))
	{
		new = ft_lstnew(NULL, line[i]);
		if (!new)
			free_tokens_things(tokens, 1);
		new->flag = DOLLAR;
		ft_lstadd_back(&tokens->temp, new);
		i++;
	}
	if (i == start)
		no_env_var(i, line, tokens, exec_part);
	if (!from_lst_a_to_lst_b(&tokens->temp, &tokens->words))
		free_tokens_things(tokens, 1);
	if (i != start)
		manage_env_var(tokens, exec_part);
	return (i);
}
