#include "minishell.h"


size_t  str_len(const char *str)
{
    size_t  i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	p;

	p = 0;
	i = 0;
	j = 0;
	if (str_len(needle) > str_len(haystack) || !needle[0] || !haystack)
		return (!needle[0] || !haystack ? (char *)haystack : NULL);
	while (haystack[i] && i < len)
	{
		while (haystack[i] != needle[j] && haystack[i] && i < len)
			i++;
		p = haystack[i] == needle[j] ? i : p;
		while (haystack[i] == needle[j]
			&& haystack[i] && needle[j++] && i++ < len)
			if (needle[j] == '\0')
				return ((char *)haystack + p);
		if (i >= len || !haystack[i])
			return (NULL);
		i = p;
		j = 0;
		i++;
	}
	return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	dest = malloc(sizeof(char) * (str_len(s1) + str_len(s2) + 1));
	if (!dest)
		return (NULL);
	while (i <= str_len(s1) && s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	while (j <= str_len(s2) && s2[j])
	{
		dest[i] = s2[j];
		i++;
		j++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strdup(const char *s1)
{
	int		i;
	char	*dest;

	i = 0;
	dest = malloc(sizeof(char) * (str_len(s1) + 1));
	if (!dest)
		return (NULL);
	while (s1[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = s1[i];
	return (dest);
}
