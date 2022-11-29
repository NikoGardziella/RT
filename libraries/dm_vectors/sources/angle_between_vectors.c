/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   angle_between_vectors.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/28 16:35:58 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/28 16:36:11 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dm_vectors.h"

double	angle_between_vectors(t_3d v1, t_3d v2)
{
	double	magn[2];

	magn[0] = vector_magnitude(v1);
	magn[1] = vector_magnitude(v2);
	return (acos(dot_product(v1, v2) / (magn[0] + magn[1])));
}
