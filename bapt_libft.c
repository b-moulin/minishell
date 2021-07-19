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

// char	*ft_strjoin(char const *s1, char const *s2)
// {
// 	char	*dest;
// 	size_t	i;
// 	size_t	j;

// 	i = 0;
// 	j = 0;
// 	if (!s1 || !s2)
// 		return (NULL);
// 	dest = malloc(sizeof(char) * (str_len(s1) + str_len(s2) + 1));
// 	if (!dest)
// 		return (NULL);
// 	while (i <= str_len(s1) && s1[i])
// 	{
// 		dest[i] = s1[i];
// 		i++;
// 	}
// 	while (j <= str_len(s2) && s2[j])
// 	{
// 		dest[i] = s2[j];
// 		i++;
// 		j++;
// 	}
// 	dest[i] = '\0';
// 	return (dest);
// }

// char	*ft_strdup(const char *s1)
// {
// 	int		i;
// 	char	*dest;

// 	i = 0;
// 	dest = malloc(sizeof(char) * (str_len(s1) + 1));
// 	if (!dest)
// 		return (NULL);
// 	while (s1[i])
// 	{
// 		dest[i] = s1[i];
// 		i++;
// 	}
// 	dest[i] = s1[i];
// 	return (dest);
// }

// void	ft_putstr_fd(char *s, int fd)
// {
// 	size_t	i;

// 	i = 0;
// 	if (!(!s || !fd))
// 	{
// 		while (s[i])
// 		{
// 			write(fd, &s[i], 1);
// 			i++;
// 		}
// 	}
// }

static long long		ft_lendest(long long n)
{
	long long	size;
	long long	count;

	size = 0;
	if (n < 0)
	{
		count = -n;
		size++;
	}
	else
		count = n;
	while (count >= 10)
	{
		size++;
		count = count / 10;
	}
	size++;
	return (size);
}

static char				*ft_filldest(long long n, long long len, char *dest)
{
	dest[len] = '\0';
	len--;
	if (n < 0)
	{
		dest[0] = '-';
		n = -n;
	}
	while (n >= 10)
	{
		dest[len] = (n % 10) + 48;
		len--;
		n = n / 10;
	}
	dest[len] = n + 48;
	return (dest);
}

char					*ft_itoa(int n)
{
	char		*dest;
	long long	lln;

	if (n == 0)
	{
		dest = malloc(sizeof(char) * 2);
		if (!dest)
			return (NULL);
		dest[1] = '\0';
		dest[0] = '0';
		return (dest);
	}
	lln = n;
	if (!lln)
		return (NULL);
	dest = malloc(sizeof(char) * ft_lendest(lln) + 1);
	if (!dest)
		return (NULL);
	return (ft_filldest(lln, ft_lendest(lln), dest));
}

long long		ft_atoi(const char *str)
{
	long	i;
	long long ret_value;
	long long save;

	i = 0;
	if (!str || !str[0])
		return (-1);
	if (str_len(str) > 19)
		return (-1);
	while (str[i])
		if (str[i] < '0' || str[i++] > '9')
			return (-1);
	i = 0;
	ret_value = str[i++] - 48;
	save = ret_value;
	// printf("str %s\n", str);
	// printf("ret_value %lld\n", ret_value);
	while (str[i])
	{
		ret_value = (ret_value * 10) + (str[i++] - 48);
		// printf("ret_value %lld\n", ret_value);
		if (ret_value < save)
			return (-1);
		save = ret_value;
	}
	return (ret_value);
}
