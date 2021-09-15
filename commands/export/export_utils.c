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

t_bool	is_valid_env_name(const char *env)
{
	size_t	i;

	i = 0;
	if ((env[i] >= 'a' && env[i] <= 'z')
		|| (env[i] >= 'A' && env[i] <= 'Z') || env[i] == '_')
	{
		i++;
		while (env[i])
		{
			if (!((env[i] >= 'a' && env[i] <= 'z')
					|| (env[i] >= 'A' && env[i] <= 'Z')
					|| env[i] == '_' || (env[i] >= '0' && env[i] <= '9')))
				return (FALSE);
			i++;
		}
	}
	else
		return (FALSE);
	return (TRUE);
}

int	find_env_var(const char *tofind, t_shell *shell)
{
	size_t	i;
	char	*ret_strnstr;
	char	*tmp;

	i = 0;
	while (shell->env[i])
	{
		tmp = ft_strjoin(tofind, "=");
		ret_strnstr = ft_strnstr(shell->env[i], tmp, str_len(tmp));
		free(tmp);
		if (ret_strnstr)
			return (i);
		i++;
	}
	return (-1);
}

int	find_env_all_var(const char *tofind, t_shell *shell)
{
	size_t	i;
	char	*ret_strnstr;
	char	*tmp;

	i = -1;
	while (shell->env_all[++i])
	{
		ret_strnstr = ft_strjoin("declare -x ", tofind);
		tmp = ft_strjoin(ret_strnstr, "=");
		free(ret_strnstr);
		ret_strnstr = ft_strnstr(shell->env_all[i], tmp, str_len(tmp));
		free(tmp);
		if (ret_strnstr)
			return (i);
	}
	i = -1;
	while (shell->env_all[++i])
	{
		tmp = ft_strjoin("declare -x ", tofind);
		ret_strnstr = ft_strnstr(shell->env_all[i], tmp, str_len(tmp));
		free(tmp);
		if (ret_strnstr)
			return (i);
	}
	return (-1);
}

void	export_no_name(const char *arg, t_fd fd, t_shell *shell)
{
	size_t	i;
	int		there_is_par;
	char	*tmp;
	char	*str;

	there_is_par = 0;
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '(' || arg[i] == ')')
			there_is_par++;
		i++;
	}
	if (there_is_par >= 1)
	{
		write(fd, "bash: syntax error near unexpected token `='\n",
			str_len("bash: syntax error near unexpected token `='\n"));
		shell->cmd_status = FAILED;
		return ;
	}
	tmp = ft_strdup("bash: syntax error near unexpected token `");
	str = ft_strjoin(tmp, arg);
	tmp = ft_strjoin(str, "'\n");
	shell->cmd_status = FAILED;
	write(fd, tmp, str_len(tmp));
}

void	export_error_token(t_export export,
	const char *name,
		t_shell *shell, const char *arg)
{
	export.str = ft_strjoin("bash: syntax error near unexpected token `",
			export.token);
	if (str_len(name) == 2 && name[0] == '(' && name[1] == ')')
	{
		export.tmp = ft_strjoin(export.str, arg);
		export.str = ft_strjoin(export.tmp, "'\n");
		write(export.fd, export.str, str_len(export.str));
		shell->cmd_status = FAILED;
		return ;
	}
	if (!arg || !arg[0])
	{
		export.tmp = ft_strjoin(export.str, "'\n");
		write(export.fd, export.tmp, str_len(export.tmp));
		shell->cmd_status = FAILED;
		return ;
	}
	export.tmp = ft_strjoin(export.str, "=");
	export.str = ft_strjoin(export.tmp, arg);
	export.tmp = ft_strjoin(export.str, "'\n");
	write(export.fd, export.tmp, str_len(export.tmp));
	shell->cmd_status = FAILED;
	return ;
}
