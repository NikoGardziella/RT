/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflect_vector.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:38:58 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/15 10:51:59 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dm_vectors.h"

t_3d	reflect_vector(t_3d v, t_3d n)
{
	t_3d	r;

	n = normalize_vector(n);
	v = normalize_vector(v);
	r = subtract_vectors(v, scale_vector(n, 2.0f * dot_product(v, n)));
	return (r);
}



