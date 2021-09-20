/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_env_rep.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: efarin <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 17:52:49 by efarin            #+#    #+#             */
/*   Updated: 2021/09/17 17:52:52 by efarin           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_hdoc(t_hdoc *hdoc, char *str)
{
	hdoc->i = 0;
	hdoc->tmp = 0;
	hdoc->len = 0;
	hdoc->start = 0;
	hdoc->save = 0;
	hdoc->env_var_name = 0;
	hdoc->result = 0;
	while (str[hdoc->i] && str[hdoc->i] != '$')
		hdoc->i++;
}

void	hdoc_start_loop(t_hdoc *hdoc, char *str)
{
	hdoc->len = 0;
	while (str[hdoc->i] && str[hdoc->i] != '$')
	{
		hdoc->i++;
		hdoc->len++;
	}
	hdoc->tmp = ft_substr(str, hdoc->start, hdoc->len);
}

void	middle_hdoc(t_hdoc *hdoc, char *str, t_shell *shell)
{
	if (str[hdoc->i] && str[hdoc->i] == '$')
	{
		hdoc->i++;
		hdoc->start = hdoc->i;
		while (str[hdoc->i] && !is_it_env_var_separator(str[hdoc->i], NULL))
			hdoc->i++;
		hdoc->env_var_name = ft_substr(str, hdoc->start, hdoc->i - hdoc->start);
		hdoc->env_var_name = get_env_arg(shell->env, hdoc->env_var_name);
		hdoc->save = ft_strjoin(hdoc->tmp, hdoc->env_var_name);
	}
	else
		hdoc->save = ft_strjoin(hdoc->result, hdoc->tmp);
	hdoc->result = ft_strjoin(hdoc->result, hdoc->save);
}

char	*parse_hirdoc_str(char *str, t_shell *shell)
{
	t_hdoc	*hdoc;

	if (!str)
		return (str);
	hdoc = malloc(sizeof(t_hdoc));
	if (!hdoc)
		exit(0);
	init_hdoc(hdoc, str);
	if (!str[hdoc->i])
		return (str);
	hdoc->i = 0;
	while (str[hdoc->i])
	{
		hdoc_start_loop(hdoc, str);
		if (!str[hdoc->i])
			break ;
		middle_hdoc(hdoc, str, shell);
		if (!hdoc->result)
			hdoc->result = hdoc->save;
		if (!str[hdoc->i])
			break ;
		hdoc->start = hdoc->i;
	}
	return (hdoc->result);
}
