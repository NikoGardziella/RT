/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   random_rangef.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 09:20:07 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/19 15:05:40 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

double	random_rangef(double min, double max)
{
	double	random;

	random = ((double)xorshift32() / UINT32_MAX);
	return (((max - min) * random) + min);
}
