#include "minishell.h"

size_t	ft_lignes(const char *s, char *dest, size_t i, size_t start)
{
	if (start >= str_len(s))
	{
		dest[i] = '\0';
		return (0);
	}
	return (1);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	maxlen;
	char	*dest;

	if (!s)
		return (NULL);
	if (start > str_len(s))
	{
		if (!(dest = malloc(sizeof(char))))
			return (NULL);
		dest[0] = '\0';
		return (dest);
	}
	len = len > str_len(s) ? str_len(s) + start : len;
	maxlen = start + len;
	i = 0;
	if (!(dest = malloc(sizeof(char) * len + 1)))
		return (NULL);
	if (ft_lignes(s, dest, i, start) == 0)
		return (dest);
	while (start < maxlen)
		dest[i++] = s[start++];
	dest[i] = '\0';
	return (dest);
}
