/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compare_ray_hits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 15:17:21 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/02 15:27:33 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	temp;

static void push_photons_to_right(t_ray_hit *photon)
{
	int		i;

	i = N_CLOSEST_PHOTONS - 1;
	while (i > 0)
	{
		photon[i] = photon[i - 1];
		i--;
	}
}

static void	find_n_nearest_photons(t_cam_hit *cam, t_list *photon_list)
{
	t_list		*photon_looper;
	t_ray_hit	*photon_hit;
	t_3d		vector;
	double		distance;
	int			i;
	
	i = 0;
	while (i < N_CLOSEST_PHOTONS)
	{
		photon_looper = photon_list;
		if (cam->photon[i].color != 0x000000)
		{
			vector = subtract_vectors(cam->hit.point, cam->photon[i].point);
			distance = vector_magnitude(vector);
		}
		else
			distance = -1.0;
		while (photon_looper != NULL)
		{
			photon_hit = (t_ray_hit *)photon_looper->content;
			vector = subtract_vectors(cam->hit.point, photon_hit->point);
			if (distance > vector_magnitude(vector) || distance < 0)
			{
				if (distance > 0)
					push_photons_to_right(cam->photon);
				cam->photon[i] = *photon_hit;
				i = -1;
				break ;
			}
			photon_looper = photon_looper->next;
		}
		i++;
	}
}

void	*compare_ray_hits(void *arg)
{
	t_multithread	*tab;
	t_scene			*scene;
	t_cam_hit		*cam;
	t_2i			coords;
	int				buffer_coords;

	tab = (t_multithread *)arg;
	scene = tab->env->scene;
	coords.y = 0;
	while (coords.y < tab->img->dim.size.y - 1)
	{
		if (coords.y % scene->resolution_range.y == tab->resolution->y)
		{
			coords.x = tab->start;
			while (coords.x < tab->end)
			{
				if (tab->nb == 0)
				{
					if (coords.x == 0 && coords.y == 0)
						temp = 1;
					else
						temp = 0;
				}
				if (coords.x % scene->resolution_range.y == tab->resolution->x)
				{
					buffer_coords = coords.y * tab->img->dim.size.x + coords.x;
					cam = &scene->cam_hit_buffer[buffer_coords];
					find_n_nearest_photons(cam, scene->photon_list[0]);
					if (coords.x == tab->img->dim.size.x / 2 && coords.y == tab->img->dim.size.y / 2)
						printf("furthest_photon %f %f %f\n", cam->photon[N_CLOSEST_PHOTONS - 1].point.x, cam->photon[N_CLOSEST_PHOTONS - 1].point.y, cam->photon[N_CLOSEST_PHOTONS - 1].point.z);
				}
				coords.x += 1;
			}
		}
		coords.y += 1;
	}
	return (NULL);
}
