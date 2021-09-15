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

void	cutcut_export_error(t_export export, t_shell *shell,
	const char *name, const char *arg)
{
	if (export.token != 0)
	{
		export_error_token(export, name, shell, arg);
		return ;
	}
	export.tmp = ft_strjoin("bash: export: `", name);
	export.tmp = ft_strjoin(export.tmp, "\': not a valid identifier\n");
	write(export.fd, export.tmp, str_len(export.tmp));
	shell->cmd_status = FAILED;
}

void	cut_export_error(t_export export, t_shell *shell,
	const char *name, const char *arg)
{
	if (export.count > 0 && export.count % 2 == 0 && export.tmp)
	{
		export.str = ft_strjoin("bash: syntax error near unexpected token `",
				export.tmp);
		export.tmp = ft_strjoin(export.str, "'\n");
		write(export.fd, export.tmp, str_len(export.tmp));
		shell->cmd_status = FAILED;
		return ;
	}
	if (export.count > 0 && export.count % 2 == 1)
	{
		export.str = ft_strjoin("bash: syntax error near unexpected token `",
				export.token);
		export.tmp = ft_strjoin(export.str, "'\n");
		write(export.fd, export.tmp, str_len(export.tmp));
		shell->cmd_status = FAILED;
		return ;
	}
	cutcut_export_error(export, shell, name, arg);
}

t_export	init_export(t_fd fd)
{
	t_export	export;

	export.i = -1;
	export.count = 0;
	export.token = 0;
	export.tmp = 0;
	export.save = 0;
	export.fd = fd;
	return (export);
}

void	export_error(const char *name, const char *arg, t_fd fd, t_shell *shell)
{
	t_export	export;

	export = init_export(fd);
	if (!(str_len(name) == 2 && name[0] == '(' && name[1] == ')'))
	{
		while (name[++export.i])
		{
			if (name[export.i] == '(' || name[export.i] == ')')
			{
				if ((++export.count == 2 && name[0] == '(') || name[0] == ')')
					export.tmp = ft_substr(name,
							export.save + 1, export.i - export.save - 1);
				if (!export.token)
				{
					export.token = ft_strdup("0");
					export.token[0] = name[export.i];
					export.token[1] = 0;
				}
				export.save = export.i;
			}
		}
		cut_export_error(export, shell, name, arg);
	}
	export_no_name(arg, fd, shell);
}
