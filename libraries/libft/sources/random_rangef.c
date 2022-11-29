/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_rangef.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 09:20:07 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/28 09:20:22 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	random_rangef(double min, double max, uint32_t *state)
{
	double	random;

	random = ((double)xorshift32(state) / UINT32_MAX);
	return (((max - min) * random) + min);
}
