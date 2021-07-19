#include "../../minishell.h"

void	env(t_list *lst, t_shell *shell, t_fd fd)
{
	size_t  i;

	i = 0;
	lst = lst->next;
	while (shell->env_all[i])
	{
		ft_putstr_fd(shell->env_all[i], fd);
		ft_putstr_fd("\n", fd);
		i++;
	}
	shell->cmd_status = SUCESS;
}