/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 15:30:28 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/15 15:58:38 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	angle_overflow(t_3d *angle)
{
	if (angle->x >= 90.0f)
		angle->x = 90.0f;
	else if (angle->x <= -90.0f)
		angle->x = -90.0f;
	if (angle->y > 360.0f)
		angle->y -= 360.0f;
	else if (angle->y < 0.0f)
		angle->y += 360.0f;
}

static int	mouse_move(void *param)
{
	t_env		*env;
	int			ret;

	env = param;
	ret = 0;
	if ((env->mouse.state & 8) == 8)
	{
		if ((env->mouse.state & 4) == 4)
		{
			angle_overflow(&env->scene->camera_angle);
			env->scene->camera_angle.x -= (double)env->mouse.move.y / 5;
			env->scene->camera_angle.y -= (double)env->mouse.move.x / 5;
			env->scene->camera->ray.forward = rotate_point(
					(t_3d){0.0f, 0.0f, -1.0f}, env->scene->camera_angle);
			ret |= 1;
		}
	}
	return (ret);
}

static void	mouse_hold_util(t_env *env, t_camera *camera, t_2i mouse_coords)
{
	t_ray		ray;

	ft_bzero(&ray, sizeof(t_ray));
	*camera = init_camera(env->img[0].dim.size, camera->ray.origin,
			camera->ray.forward, camera->fov);
	ray = get_ray(mouse_coords, &env->img[0], camera);
	ray.hit_point = add_vectors(env->scene->camera->ray.origin,
			scale_vector(ray.forward, env->sel_ray.distance));
	env->sel_ray.object->origin = add_vectors(
			env->sel_ray.object->origin,
			subtract_vectors(ray.hit_point, env->sel_ray.hit_point));
	env->sel_ray.object->end = add_vectors(env->sel_ray.object->origin,
			scale_vector(env->sel_ray.object->axis,
				env->sel_ray.object->axis_length));
	env->sel_ray.forward = ray.forward;
	env->sel_ray.hit_point = ray.hit_point;
}

static int	mouse_hold(void *param)
{
	t_env		*env;
	int			ret;
	t_camera	*camera;
	t_2i		mouse_coords;

	env = param;
	camera = env->scene->camera;
	SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y);
	ret = 0;
	if (((env->mouse.state & 9) == 1 && env->keymap != 0)
		|| ((env->mouse.state & 9) == 9))
	{
		if (env->sel_ray.object != NULL && env->sel_element == 1)
		{
			mouse_hold_util(env, camera, mouse_coords);
			ret |= 1;
		}
	}
	return (ret);
}

int	mouse_main(void *param)
{
	int		ret;
	t_env	*env;
	t_2i	curr_mouse;

	env = param;
	ret = 0;
	SDL_GetMouseState(&curr_mouse.x, &curr_mouse.y);
	env->mouse.move.x = curr_mouse.x - env->mouse.pos.x;
	env->mouse.move.y = curr_mouse.y - env->mouse.pos.y;
	env->mouse.pos = curr_mouse;
	if (env->mouse.move.x == 0 && env->mouse.move.y == 0)
		env->mouse.state = env->mouse.state & (uint8_t)(~(8));
	ret |= mouse_move(param);
	ret |= mouse_hold(param);
	ret |= mouse_scroll(param);
	return (ret);
}
