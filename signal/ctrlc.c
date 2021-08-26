/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 11:53:28 by bmoulin           #+#    #+#             */
/*   Updated: 2021/08/26 11:53:31 by bmoulin          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	middle_cc(t_cc *cc)
{
	if (cc->cmd[0] == 0)
	{
		printf("minishell                              \nminishell ");
		rl_redisplay();
		rl_replace_line("", 0);
	}
	else
	{
		rl_replace_line("", 0);
		rl_redisplay();
		if (str_len(cc->cmd) <= 11)
			printf("minishell ");
		printf("%s", cc->cmd);
		printf("  \n");
		printf("minishell ");
	}
}

void	ctrl_c(int sig)
{
	t_cc	*cc;

	cc = malloc(sizeof(t_cc));
	if (!cc)
		exit(1);
	cc->cmd = ft_strdup(rl_line_buffer);
	cc->i = str_len(cc->cmd);
	if (sig == SIGINT)
	{
		if (g_normal_shell == 0)
		{
			rl_replace_line("", 0);
			rl_redisplay();
			if (str_len(cc->cmd) <= 11)
				printf("> ");
			printf("%s", cc->cmd);
			printf("  \n");
			printf("minishell ");
		}
		else
			middle_cc(cc);
		g_normal_shell = 1;
	}
	free(cc->cmd);
}
