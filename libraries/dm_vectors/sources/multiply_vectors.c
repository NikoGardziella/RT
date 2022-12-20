/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiply_vectors.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 18:24:56 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/19 18:25:17 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dm_vectors.h"

t_3d	multiply_vectors(t_3d vector1, t_3d vector2)
{
	t_3d	multiplied;

	multiplied.x = vector1.x * vector2.x;
	multiplied.y = vector1.y * vector2.y;
	multiplied.z = vector1.z * vector2.z;
	return (multiplied);
}
