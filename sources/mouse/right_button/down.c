/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   down.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 12:39:02 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/02 13:49:36 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/*void	rot_overflows(t_env *env)
{
	if (env->cam.rot.x > 90)
		env->cam.rot.x = 90;
	else if (env->cam.rot.x < -90)
		env->cam.rot.x = -90;
	if (env->cam.rot.y > 360)
		env->cam.rot.y = 0;
	else if (env->cam.rot.y < 0)
		env->cam.rot.y = 360;
	if (env->cam.rot.z > 360)
		env->cam.rot.z = 0;
	else if (env->cam.rot.z < 0)
		env->cam.rot.z = 360;
}*/

/*
	env->cam.rot.y -= (double)env->mouse.move.x / 4;
	env->cam.rot.x -= (double)env->mouse.move.y / 4;
	rot_overflows(env);
	process_image(&env->sdl, &env->img[0], 1, env);
	process_image(&env->sdl, &env->img[1], 1, env);
	process_image(&env->sdl, &env->img[2], 1, env);
	dim[1].start = coords;
	blit_surface(test, &dim[0], env->sdl.screen, &dim[1]);
	SDL_UpdateWindowSurface(env->sdl.window);
	printf("LEFT CLICK\n");
	t_dim	dim[2];
	SDL_Surface *test;
	//test = SDL_ConvertSurfaceFormat(test, SDL_PIXELFORMAT_ARGB8888, 0);
*/

void	right_button_down(t_2i coords, void *param)
{
	t_env	*env;

	env = param;
	env->mouse_state |= 4;
	(void)coords;
	printf("RIGHT DOWN\n");
}
