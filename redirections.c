#include "minishell.h"

int	check_fd_redir(char *word)
{
	int		i;

	i = 0;
	while (word[i] && word[i] >= 48 && word[i] <= 57)
		i++;
	if (!word[i])
		return (1);
	return (0);
}
