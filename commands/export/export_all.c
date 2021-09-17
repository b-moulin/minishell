/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   export_all.c                                     .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: aviscogl <aviscogl@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2021/08/26 02:25:21 by bmoulin      #+#   ##    ##    #+#       */
/*   Updated: 2021/09/02 23:29:57 by aviscogl    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */
#include "../../minishell.h"

void	end_export_add_arg_all(const char *arg,
	t_exargall exargall, t_shell *shell)
{
	if (arg == NULL)
	{
		exargall.new_env[exargall.i] = exargall.tmp;
		exargall.new_env[exargall.i + 1] = NULL;
		ft_freee(shell->env_all);
		shell->env_all = exargall.new_env;
		return ;
	}
	exargall.save = ft_strjoin(exargall.tmp, "=\"");
	free(exargall.tmp);
	exargall.tmp = ft_strjoin(exargall.save, arg);
	free(exargall.save);
	exargall.new_env[exargall.i] = ft_strjoin(exargall.tmp, "\"");
	free(exargall.tmp);
	exargall.new_env[exargall.i + 1] = NULL;
	ft_freee(shell->env_all);
	shell->env_all = exargall.new_env;
}

void	export_add_arg_all(const char *name, const char *arg, t_shell *shell)
{
	t_exargall	exargall;

	exargall.i = -1;
	while (shell->env_all[++exargall.i])
		;
	exargall.new_env = malloc(sizeof(char *) * (exargall.i + 2));
	if (!exargall.new_env)
		return ;
	exargall.i = 0;
	while (shell->env_all[exargall.i])
	{
		exargall.new_env[exargall.i] = ft_strdup(shell->env_all[exargall.i]);
		exargall.i++;
	}
	exargall.tmp = ft_strjoin("declare -x ", name);
	end_export_add_arg_all(arg, exargall, shell);
}

void	export_all(const char *name, const char *arg, t_shell *shell)
{
	int		i;
	char	*tmp;
	char	*env_alli;

	i = find_env_all_var(name, shell);
	if (i == -1)
		export_add_arg_all(name, arg, shell);
	if (i != -1)
	{
		tmp = ft_strjoin("declare -x ", name);
		if (arg == NULL)
		{
			shell->env_all[i] = tmp;
			return ;
		}
		env_alli = ft_strjoin(tmp, "=\"");
		free(tmp);
		tmp = ft_strjoin(env_alli, arg);
		free(env_alli);
		env_alli = ft_strjoin(tmp, "\"");
		free(tmp);
		free(shell->env_all[i]);
		shell->env_all[i] = env_alli;
	}
}
