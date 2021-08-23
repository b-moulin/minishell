#include "../../minishell.h"

void	cd_tild_inpath(char *path, t_shell  *shell)
{
	char	*tmp;

	tmp = get_env_arg(shell->env, "HOME");
	(path)[0] = '/';
	path = ft_strjoin(tmp, path);
}

void	cd(t_list *lst, t_shell *shell, t_fd fd)
{
	DIR		*rep;
	char	*str;
	char	*path;
	// printf("ICI !\n");
	if (lst->lst_struct->exec->next == NULL)
	{
		shell->cmd_status = SUCESS;
		return ;
	}
	path = lst->lst_struct->exec->next->content.word;
	if (path[0] == '~')
		cd_tild_inpath(path, shell);
	rep = opendir(path);
	chdir(path);
	if (rep == NULL)
	{
		str = ft_strdup("bash: cd: ");
		str = ft_strjoin(str, path);
		str = ft_strjoin(str, ": No such file or directory\n");
		if (fd == 1)
			write(2, str, str_len(str));
		else
			write(fd, str, str_len(str));
		shell->cmd_status = FAILED;
		return ;
	}
	closedir(rep);
	shell->cmd_status = SUCESS;
}
