/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hold.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 15:53:27 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/15 15:10:15 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	move_camera(t_env *env)
{
	t_camera	*camera;

	camera = env->scene->camera;
	if ((env->keymap & 0xFFFFFFFF) == 0)
		return (0);
	if ((env->keymap & KEY_A) == KEY_A)
		camera->ray.origin = add_vectors(camera->ray.origin,
				scale_vector(camera->right, -1));
	if ((env->keymap & KEY_W) == KEY_W)
		camera->ray.origin = add_vectors(camera->ray.origin,
				camera->ray.forward);
	if ((env->keymap & KEY_D) == KEY_D)
		camera->ray.origin = add_vectors(camera->ray.origin,
				camera->right);
	if ((env->keymap & KEY_S) == KEY_S)
		camera->ray.origin = add_vectors(camera->ray.origin,
				scale_vector(camera->ray.forward, -1));
	if ((env->keymap & KEY_SPACE) == KEY_SPACE)
		camera->ray.origin = add_vectors(camera->ray.origin,
				(t_3d){0.0f, 1.0f, 0.0f});
	if ((env->keymap & KEY_LSHIFT) == KEY_LSHIFT)
		camera->ray.origin = add_vectors(camera->ray.origin,
				(t_3d){0.0f, -1.0f, 0.0f});
	return (1);
}

int	keyboard_hold(t_env *env)
{
	int	ret;

	ret = 0;
	ret |= move_camera(env);
	return (ret);
}
