/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   down.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 12:39:02 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/11 16:45:04 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	coords_in_area(t_dim dim, t_2i coords)
{
	if (coords.x > dim.start.x && coords.x < dim.start.x + dim.size.x
			&& coords.y > dim.start.y && coords.y < dim.start.y + dim.size.y)
		return (1);
	return (0);
}

void	left_button_down(void *param)
{
	t_env		*env;
	t_2i		mouse_coords;
	t_camera	*camera;
	t_hit		hit;

	env = param;
	camera = env->scene->camera;
	env->mouse.state |= 1;
	SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y);
	if (coords_in_area(env->img[3].dim, mouse_coords) && env->sidebar == 1)
		printf("Placeholder\n");
	else if (coords_in_area(env->img[1].dim, mouse_coords))
		env->sidebar = 1;
	else if (coords_in_area(env->img[0].dim, mouse_coords))
	{
		ft_bzero(&env->sel_ray, sizeof(t_ray));
		*camera = init_camera(env->img[0].dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
		env->sel_ray = get_ray(mouse_coords, &env->img[0], camera);
		env->sel_ray.object = NULL;
		raycast(&env->sel_ray, env->scene, &hit);
	}
	else
		return ;
	put_images_to_screen(env);
}
