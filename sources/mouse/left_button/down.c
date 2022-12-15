/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   down.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 12:39:02 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/07 11:17:02 by dmalesev         ###   ########.fr       */
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

	env = param;
	camera = env->scene->camera;
	env->mouse.state |= 1;
	SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y);
	if (coords_in_area(env->img[1].dim, mouse_coords))
	{
		env->sidebar = (int8_t)(env->sidebar * -1);
		//ft_bzero(&env->sel_ray, sizeof(t_ray));
		//render_screen(env);
	}
	else if (coords_in_area(env->img[6].dim, mouse_coords) && env->sel_ray.object != NULL && env->sidebar == 1)
	{
		env->sel_element = 2;
	}
	else if (coords_in_area(env->img[7].dim, mouse_coords) && env->sel_ray.object->type == LIGHT && env->sidebar == 1)
	{
		printf("lumen slider");
		env->sel_element = 5;
	}
	else if (coords_in_area(env->img[6].dim, mouse_coords) && env->sel_ray.object->type == LIGHT && env->sidebar == 1)
	{
		printf("intensity slider");
		env->sel_element = 6;
	}
	else if (coords_in_area(env->img[7].dim, mouse_coords) && env->sel_ray.object != NULL && env->sidebar == 1)
	{
		env->sel_element = 3;
	}
	else if (coords_in_area(env->img[8].dim, mouse_coords) && env->sel_ray.object != NULL && env->sidebar == 1)
	{
		env->sel_element = 4;
	}
	else if (coords_in_area(env->img[3].dim, mouse_coords) && env->sidebar == 1)
		printf("Placeholder\n");
	else if (coords_in_area(env->img[0].dim, mouse_coords))
	{
		env->sel_element = 1;
		ft_bzero(&env->sel_ray, sizeof(t_ray));
		*camera = init_camera(env->img[0].dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
		env->sel_ray = get_ray(mouse_coords, &env->img[0], camera);
		env->sel_ray.object = NULL;
		raycast(&env->sel_ray, env->scene, 1);
		//if(env->sel_ray.object != NULL)
		//env->sidebar = 1;
		//render_screen(env);
	}
	else
		return ;
	if(env->sel_ray.object == NULL && env->sidebar == 1)
	{
		env->sidebar = -1;
		put_images_to_screen(env);
	}
}
