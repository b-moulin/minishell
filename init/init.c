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

#include "../minishell.h"

char	*get_env_name(char *env)
{
	size_t	i;

	i = 0;
	while (env[i] && env[i] != '=')
		i++;
	return (ft_substr(env, 0, i));
}

void	init_init_env_all(t_envinit *envinit)
{
	envinit->i = 0;
	envinit->j = 0;
	envinit->env_name = 0;
	envinit->tmp = 0;
	envinit->save = 0;
	envinit->env_arg = 0;
}

void	loop_init_env_all(t_shell *shell, t_envinit *envinit)
{
	envinit->env_name = get_env_name(shell->env[envinit->i]);
	envinit->env_arg = get_env_arg(shell->env, envinit->env_name);
	envinit->tmp = ft_strjoin("\"", envinit->env_arg);
	envinit->save = ft_strjoin("declare -x ", envinit->env_name);
	shell->env_all[envinit->j] = ft_strjoin(envinit->save, "=");
	free(envinit->save);
	envinit->save = ft_strjoin(shell->env_all[envinit->j], envinit->tmp);
	free(shell->env_all[envinit->j]);
	shell->env_all[envinit->j] = ft_strjoin(envinit->save, "\"");
	free(envinit->env_name);
	free(envinit->env_arg);
	free(envinit->tmp);
	free(envinit->save);
	envinit->j++;
	envinit->i++;
}

void	init_env_all(t_shell *shell)
{
	t_envinit	*envinit;

	envinit = malloc(sizeof(t_envinit));
	if (!envinit)
		exit(1);
	init_init_env_all(envinit);
	while (shell->env[envinit->i])
		envinit->i++;
	shell->env_all = malloc(sizeof(char *) * (envinit->i + 1));
	envinit->i = 0;
	while (shell->env[envinit->i])
	{
		if (envinit->i == find_env_var("_", shell))
			envinit->i++;
		if (!shell->env[envinit->i])
			break ;
		loop_init_env_all(shell, envinit);
	}
	shell->env_all[envinit->j] = NULL;
	free(envinit);
}

void	init_env(char **env, t_shell *shell)
{
	size_t	i;

	i = 0;
	while (env[i])
		i++;
	i++;
	shell->env = malloc(sizeof(char *) * i);
	if (!shell->env)
		return ;
	i = 0;
	while (env[i])
	{
		shell->env[i] = ft_strdup(env[i]);
		i++;
	}
	shell->env[i] = NULL;
	init_env_all(shell);
	shell->read_fd = -1;
}
