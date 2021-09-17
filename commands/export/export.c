/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_error.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmoulin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/26 02:25:21 by bmoulin           #+#    #+#             */
/*   Updated: 2021/08/26 02:25:24 by bmoulin          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void	end_export_add_arg(t_exarg axarg, t_shell *shell,
	const char *name, const char *arg)
{
	while (shell->env[++axarg.i])
		;
	axarg.new_env = malloc(sizeof(char *) * (axarg.i + 2));
	if (!axarg.new_env)
		return ;
	axarg.i = 0;
	while (shell->env[axarg.i])
	{
		axarg.new_env[axarg.i] = ft_strdup(shell->env[axarg.i]);
		axarg.i++;
	}
	axarg.tmp = ft_strjoin(name, "=");
	axarg.new_env[axarg.i] = ft_strjoin(axarg.tmp, arg);
	free(axarg.tmp);
	axarg.new_env[axarg.i + 1] = NULL;
	ft_freee(shell->env);
	shell->env = axarg.new_env;
}

void	export_add_arg(const char *name,
	const char *arg, t_shell *shell, t_fd fd)
{
	t_exarg	axarg;

	axarg.i = -1;
	if (!is_valid_env_name(name))
	{
		while (name[++axarg.i])
			if (name[axarg.i] == '(' || name[axarg.i] == ')')
				export_error(name, arg, fd, shell);
		axarg.tmp = ft_strdup("bash: export: `");
		axarg.tmp = ft_strjoin(axarg.tmp, name);
		if (arg)
			axarg.tmp = ft_strjoin(axarg.tmp, "=");
		if (arg)
			axarg.tmp = ft_strjoin(axarg.tmp, arg);
		axarg.tmp = ft_strjoin(axarg.tmp, "': not a valid identifier\n");
		write(fd, axarg.tmp, str_len(axarg.tmp));
		shell->cmd_status = FAILED;
		return ;
	}
	end_export_add_arg(axarg, shell, name, arg);
}

void	end_export(t_exprt *exprt, t_shell *shell, t_fd fd)
{
	exprt->i = find_env_var(exprt->name, shell);
	if (exprt->i == -1)
	{
		export_add_arg(exprt->name, exprt->arg, shell, fd);
	}
	else
	{
		exprt->tmp = ft_strjoin(exprt->name, "=");
		free(shell->env[exprt->i]);
		shell->env[exprt->i] = ft_strjoin(exprt->tmp, exprt->arg);
		free(exprt->tmp);
	}
	export_all(exprt->name, exprt->arg, shell);
	shell->cmd_status = SUCESS;
	free(exprt->name);
	free(exprt->arg);
	free(exprt);
}

void	middle_export(t_exprt *exprt)
{
	if (exprt->exec->content.word)
	{
		while (exprt->exec->content.word[exprt->i])
		{
			if (exprt->exec->content.word[exprt->i] == '=')
				break ;
			exprt->i++;
		}
		if (exprt->exec->content.word[exprt->i])
		{
			exprt->name = ft_substr(exprt->exec->content.word, 0, exprt->i);
			if ((size_t)exprt->i == str_len(exprt->exec->content.word)
				&& exprt->exec->content.word[exprt->i - 1] == '=')
			{
				exprt->arg = ft_strdup("");
			}
			else
				exprt->arg = ft_substr(exprt->exec->content.word, exprt->i + 1,
						str_len(exprt->exec->content.word) - exprt->i - 1);
		}
		else
			exprt->name = ft_strdup(exprt->exec->content.word);
	}
}

void	export(t_list *lst, t_shell *shell, t_fd fd)
{
	t_exprt	*exprt;

	exprt = malloc(sizeof(t_exprt));
	if (!exprt)
		exit(1);
	exprt->name = 0;
	exprt->arg = 0;
	exprt->i = 0;
	exprt->exec = lst->lst_struct->exec;
	if (!exprt->exec->next)
	{
		env_all(lst, shell, fd);
		return ;
	}
	exprt->exec = exprt->exec->next;
	middle_export(exprt);
	end_export(exprt, shell, fd);
}
