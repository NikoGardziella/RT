/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/11 15:30:28 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/11 16:19:10 by dmalesev         ###   ########.fr       */
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
	t_camera	*camera;
	t_2i		mouse_coords;
	t_hit		hit;
	t_ray		ray;

	env = param;
	camera = env->scene->camera;
	SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y);
	ret = 0;
	if ((env->mouse.state & 8) == 8)
	{
		if ((env->mouse.state & 1) == 1)
		{
			*camera = init_camera(env->img[0].dim.size, camera->ray.origin, camera->ray.forward, camera->fov);
			ray = get_ray(mouse_coords, &env->img[0], camera);
			hit.object = NULL;
			raycast(&ray, env->scene, &hit);
			if (hit.object != NULL)
				env->sel_object = hit.object;
			ret |= 1;
		}
		if ((env->mouse.state & 4) == 4)
		{
			angle_overflow(&env->scene->camera_angle);
			env->scene->camera->ray.forward = rotate_point((t_3d){0.0f, 0.0f, -1.0f}, env->scene->camera_angle);
			env->scene->camera_angle.x -= (double)env->mouse.move.y / 5;
			env->scene->camera_angle.y -= (double)env->mouse.move.x / 5;
			//env->scene->camera->ray.forward = rotate_point(env->scene->camera->ray.forward, (t_3d){0.0f, -env->sdl.event.motion.xrel / 5, 0.0f});
			//env->scene->camera->ray.forward = rotate_point(env->scene->camera->ray.forward, (t_3d){-env->sdl.event.motion.yrel / 5, 0.0f, 0.0f});
			ret |= 1;
		}
	}
	return (ret);
}

static int	mouse_hold(void *param)
{
	t_env	*env;
	int		ret;

	env = param;
	ret = 0;
	if ((env->mouse.state & 1) == 1)
	{
	}
	return (ret);
}

int	mouse_main(void *param)
{
	t_env	*env;
	int		ret;
	t_2i	curr_mouse;

	env = param;
	ret = 0;
	SDL_GetMouseState(&curr_mouse.x, &curr_mouse.y);
	env->mouse.move.x = curr_mouse.x - env->mouse.pos.x;
	env->mouse.move.y = curr_mouse.y - env->mouse.pos.y;
	//if (time_since_success(0.001, 2) >= 0.001)
		env->mouse.pos = curr_mouse;
	if (env->mouse.move.x == 0 && env->mouse.move.y == 0)
		env->mouse.state = env->mouse.state & ~(8);
	ret |= mouse_move(param);
	ret |= mouse_hold(param);
	return (ret);
}
