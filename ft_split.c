#include "minishell.h"

char	*ft_free(char **tab, size_t i)
{
	while (i-- > 0)
		free(tab[i]);
	free(tab);
	return (NULL);
}

size_t	ft_cmpt(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] == c)
			i++;
		if (s[i] != c && s[i] != '\0')
			count++;
		while (s[i] != c && s[i] != '\0')
			i++;
	}
	return (count);
}

char	**ft_mem(char const *s, char c)
{
	char	**tab;
	size_t	k;
	size_t	j;
	size_t	i;

	i = -1;
	k = 0;
	j = 0;
	tab = malloc((ft_cmpt(s, c) + 1) * sizeof(char *));
	if (!(tab))
		return (NULL);
	while (s[k] && ++i != ft_cmpt(s, c))
	{
		j = 0;
		while (s[k] == c)
			k++;
		while (s[k] && s[k] != c && s[k++])
			j++;
		tab[i] = malloc((j + 1) * sizeof(char));
		if (!(tab[i]))
			return ((char **)ft_free(tab, i));
	}
	return (tab);
}

char	**ft_split(char const *s, char c)
{
	char	**tab;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	k = 0;
	if (s == 0)
		return (NULL);
	tab = ft_mem(s, c);
	if (tab == NULL)
		return (NULL);
	while (s[k] && i != ft_cmpt(s, c))
	{
		j = 0;
		while (s[k] == c)
			k++;
		while (s[k] != c && s[k])
			tab[i][j++] = s[k++];
		tab[i][j] = '\0';
		i++;
	}
	tab[i] = 0;
	return (tab);
}
