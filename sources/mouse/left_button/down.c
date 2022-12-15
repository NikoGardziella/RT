/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   down.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 12:39:02 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/15 16:10:04 by pnoutere         ###   ########.fr       */
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

void	left_button_check(t_env *env, t_2i mouse_coords, t_camera	*camera)
{
	env->sel_element = 1;
	ft_bzero(&env->sel_ray, sizeof(t_ray));
	*camera = init_camera(env->img[0].dim.size,
			camera->ray.origin, camera->ray.forward, camera->fov);
	env->sel_ray = get_ray(mouse_coords, &env->img[0], camera);
	env->sel_ray.object = NULL;
	raycast(&env->sel_ray, env->scene, 1);
	render_screen(env);
}

void	left_button_down_checks(t_env *env, t_2i mouse_coords, t_camera	*camera)
{
	if (coords_in_area(env->img[1].dim, mouse_coords))
		env->sidebar = (int8_t)(env->sidebar * -1);
	else if (coords_in_area(env->img[6].dim, mouse_coords)
		&& env->sel_ray.object != NULL && env->sidebar == 1)
		env->sel_element = 2;
	else if (coords_in_area(env->img[7].dim, mouse_coords)
		&& env->sel_ray.object->type == LIGHT && env->sidebar == 1)
		env->sel_element = 5;
	else if (coords_in_area(env->img[7].dim, mouse_coords)
		&& env->sel_ray.object != NULL && env->sidebar == 1)
		env->sel_element = 3;
	else if (coords_in_area(env->img[8].dim, mouse_coords)
		&& env->sel_ray.object != NULL && env->sidebar == 1)
		env->sel_element = 4;
	else if (coords_in_area(env->img[0].dim, mouse_coords))
	{
		left_button_check(env, mouse_coords, camera);
	}
	else
		return ;
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
	left_button_down_checks(env, mouse_coords, camera);
	if (env->sel_ray.object == NULL && env->sidebar == 1)
	{
		env->sidebar = -1;
		put_images_to_screen(env);
	}
}
