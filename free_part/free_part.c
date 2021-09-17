/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   free_part.c                                      .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aviscogl <aviscogl@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2021/08/26 02:25:21 by bmoulin      #+#   ##    ##    #+#       */
/*   Updated: 2021/09/02 23:25:01 by aviscogl    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */
#include "../minishell.h"

void	free_double_tab(char **tab)
{
	int	i;

	i = 0;
	if (!tab)
		return ;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	free_all_env(t_shell *shell)
{
	free_double_tab(shell->env);
	free_double_tab(shell->env_all);
}

static t_lst	**wrgetter(void)
{
	static t_lst	*wrm;

	return (&wrm);
}

void	wrdestroy(void)
{
	t_lst	*current;
	t_lst	*next;
	t_lst	**wrstart;

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
	t_lst	*new_elem;

	buffer = malloc(size);
	if (!(buffer))
	{
		wrdestroy();
		return (0);
	}
	new_elem = malloc(sizeof(t_lst));
	if (!(new_elem))
	{
		free(buffer);
		wrdestroy();
		return (0);
	}
	new_elem->content = buffer;
	new_elem->next = 0;
	ft_lstadd_backbis(wrgetter(), new_elem);
	return (buffer);
}
