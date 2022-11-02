/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   down.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 12:39:02 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/02 13:59:31 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	left_button_down(t_2i coords, void *param)
{
	t_env	*env;
	env = param;
	/*t_dim	dim[2];
	SDL_Surface *test;

	test = SDL_LoadBMP("test.bmp");
	//test = SDL_ConvertSurfaceFormat(test, SDL_PIXELFORMAT_ARGB8888, 0);

	dim[0].start = (t_2i){0, 0};
	dim[0].size = (t_2i){1000, 1000};

	dim[1].start = (t_2i){0, 0};
	dim[1].size = (t_2i){500, 500};

	process_image(&env->sdl, &env->img[0], 1, env);
	process_image(&env->sdl, &env->img[1], 1, env);
	process_image(&env->sdl, &env->img[2], 1, env);
	dim[1].start = coords;
	blit_surface(test, &dim[0], env->sdl.screen, &dim[1]);
	SDL_UpdateWindowSurface(env->sdl.window);*/
	(void)coords;
	env->mouse_state |= 1;
	printf("LEFT DOWN\n");
}
