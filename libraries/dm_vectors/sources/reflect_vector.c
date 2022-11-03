/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflect_vector.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 15:38:58 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/03 15:00:26 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "dm_vectors.h"

t_3d	reflect_vector(t_3d v, t_3d n)
{
	t_3d	r;

	n = normalize_vector(n);
	r = subtract_vectors(v, scale_vector(n, 2.0f * dot_product(v, n)));
	return (r);
}
