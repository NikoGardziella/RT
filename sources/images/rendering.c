/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 14:38:21 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/01 16:38:07 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_uint	shade(t_scene *scene, t_hit *hit)
{
	t_uint	color = 0xFF00FF;

	(void)hit;
	(void)scene;
	return (color);
}

t_uint	raycast(t_ray *ray, t_scene *scene, t_hit *hit)
{
	t_uint	color;

	color = scene->ambient_color;
	// printf("%f %f %f\n", ray->direction.x, ray->direction.y, ray->direction.z);
	if (intersects(ray, scene, hit))
	{
		color = shade(scene, hit);
	}
	return (color);
}

void	render_scene(t_img *img, t_scene *scene)
{
	t_ray	ray;
	t_2i	screen;
	t_2i	cur;
	t_hit	hit;
	t_uint	color;
	printf("here\n");
	init_camera(scene->camera, scene->camera->pos, scene->camera->look_at, \
		scene->camera->fov);
	cur.y = 0;
	printf("here2\n");
	while (cur.y < img->dim.size.y)
	{
		printf("here3\n");
		cur.x = 0;
		while (cur.x < img->dim.size.x)
		{
			printf("here4\n");
			screen.x = cur.x / SCREEN_X;
			screen.y = cur.y / SCREEN_Y;
			ray = get_camera_ray(scene->camera, screen.x, screen.y);
			color = raycast(&ray, scene, &hit);
			put_pixel(screen, color, img);
			cur.x++;
		}
		cur.y++;
	}
}
