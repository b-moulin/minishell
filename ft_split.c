#include "minishell.h"

// size_t	ft_firstishere(const char *s, char c, int z)
// {
// 	size_t	i;
// 	size_t	len;

// 	len = 0;
// 	i = 0;
// 	if (z == 0)
// 	{
// 		if (s[0] == c)
// 			return (0);
// 		while (s[i] && s[i] != c)
// 			i++;
// 		if (s[i] == c && i - 1 > 0)
// 			return (1);
// 		if (s[i] == '\0')
// 			return (2);
// 		return (0);
// 	}
// 	else
// 	{
// 		while (s[i] != c && s[i++])
// 			len++;
// 		return (len);
// 	}
// }

// size_t	ft_start(const char *s, char c)
// {
// 	size_t	i;

// 	i = 0;
// 	while (s[i] && s[i] != c)
// 		i++;
// 	if (s[i] == '\0')
// 		return (0);
// 	while (s[i] && s[i] == c)
// 		i++;
// 	return (i);
// }

// size_t	ft_size(const char *s, char c)
// {
// 	int		i;
// 	size_t	size;

// 	i = 0;
// 	size = 0;
// 	if (s[i] == 0)
// 		return (0);
// 	while (s[i])
// 	{
// 		if (s[i] == c && s[i - 1] != c && i - 1 > 0)
// 			size++;
// 		i++;
// 	}
// 	if (s[i - 1] != c)
// 		size++;
// 	return (size);
// }

// size_t	ft_toutca(char const *s, char c, char **dest)
// {
// 	size_t	i;
// 	size_t	len;
// 	size_t	size;
// 	size_t	start;

// 	i = 0;
// 	start = 0;
// 	len = 0;
// 	size = ft_size(s, c);
// 	if (ft_firstishere(s, c, 0) == 1)
// 	{
// 		len = ft_firstishere(s, c, 3);
// 		if (!(dest[i] = ft_substr(s, 0, len)))
// 			return (0);
// 		i++;
// 	}
// 	while (i < size)
// 	{
// 		start = start + ft_start(s + start, c);
// 		len = ft_firstishere(s + start, c, 3);
// 		if (!(dest[i] = ft_substr(s, start, len)))
// 			return (0);
// 		i++;
// 	}
// 	return (i);
// }

// char	**ft_split(char const *s, char c)
// {
// 	char	**dest;
// 	size_t	size;
// 	size_t	i;

// 	if (!s)
// 		return (NULL);
// 	i = 0;
// 	size = ft_size(s, c);
// 	if (!(dest = malloc(sizeof(char *) * (size + 1))))
// 		return (NULL);
// 	if (size == 0)
// 	{
// 		if (!(dest[i] = malloc(sizeof(char))))
// 			return (NULL);
// 		dest[i] = NULL;
// 		return (dest);
// 	}
// 	if (ft_toutca(s, c, dest) == 0)
// 		return (NULL);
// 	i = ft_toutca(s, c, dest);
// 	if (!(dest[i] = malloc(sizeof(char))))
// 		return (NULL);
// 	dest[i] = NULL;
// 	return (dest);
// }

char	*ft_freee(char **tab, size_t i)
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
			return ((char **)ft_freee(tab, i));
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
