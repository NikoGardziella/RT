/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnequ.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/16 08:56:08 by dmalesev          #+#    #+#             */
/*   Updated: 2021/11/29 15:58:48 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strnequ(char const *s1, char const *s2, size_t n)
{
	if (!n)
		return (1);
	if (s1 && s2 && n--)
	{
		while (*s1 == *s2 && *s1 && *s2 && n--)
		{
			s1++;
			s2++;
		}
		if (*s1 == *s2)
			return (1);
	}
	return (0);
}
