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

long long	ft_atoi(const char *str)
{
	long						i;
	unsigned long long			a;
	long long					neg;

	i = 0;
	a = 0;
	neg = 1;
	while ((str[i] >= 9) && ((str[i] <= 13) || (str[i] == 32)))
		i++;
	if ((str[i] == '-' || str[i] == '+'))
	{
		if (str[i] == '-')
			neg = -neg;
		i++;
	}
	while (((str[i] >= '0') && (str[i] <= '9')) && (str[i]))
		a = a * 10 + (str[i++] - '0');
	if ((a > __LONG_LONG_MAX__) && (neg > 0))
		return (-1);
	else if ((a > __LONG_LONG_MAX__) && (neg < 0))
		return (0);
	return (a * neg);
}

char	*ft_free(char **tab, size_t i)
{
	while (i-- > 0)
		free(tab[i]);
	free(tab);
	return (NULL);
}
