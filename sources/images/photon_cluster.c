/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   photon_cluster.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 12:33:25 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/05 14:52:34 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double get_smallest_photon_cluster(t_cam_hit *hit_buffer)
{
	t_2i	coords;
	double	closest;
	double	temp;
	int		i;

	coords.y = 0;
	closest = T_MAX;
	while (coords.y < SCREEN_Y)
	{
		coords.x = 0;
		while (coords.x < SCREEN_X)
		{
			i = 0;
			temp = 0;
			while(i < N_CLOSEST_PHOTONS)
			{
				if (hit_buffer[coords.x + coords.y * SCREEN_X].photon[i].color != 0x000000)
					temp += vector_magnitude(subtract_vectors(hit_buffer[coords.x + coords.y * SCREEN_X].hit.point, hit_buffer[coords.x + coords.y * SCREEN_X].photon[i].point));
				else
				{
					temp = T_MAX;
					break ;
				}
				i++;
			}
			temp /= i;
			if (temp < closest)
			{
				closest = temp;
			}
			coords.x += 1;
		}
		coords.y += 1;
	}
	return (closest);
}
