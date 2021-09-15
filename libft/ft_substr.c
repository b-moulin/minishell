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

#include "../minishell.h"

static int	ft_smaller(int a, int b)
{
	if (a >= b)
		return (b);
	return (a);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	i;

	i = 0;
	if (start >= str_len(s))
	{
		dest = malloc(sizeof(char));
		if (dest == NULL)
			return (NULL);
		*dest = 0;
		return (dest);
	}
	dest = malloc(sizeof(char) * (ft_smaller(str_len(s), len) + 1));
	if (dest == NULL)
		return (NULL);
	if (s)
	{
		i = -1;
		while (++i < len && s[start + i])
			dest[i] = s[start + i];
	}
	dest[i] = 0;
	return (dest);
}
