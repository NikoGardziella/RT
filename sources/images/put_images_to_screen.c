/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_images_to_screen.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:05:29 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/15 11:59:16 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	put_images_to_screen(t_env *env)
{
	if (env->scene->resolution.x == 0 && env->scene->resolution.y == 0)
		process_image(&env->sdl, &env->img[0], 0, env);
	else
		process_image(&env->sdl, &env->img[0], 1, env);
	process_image(&env->sdl, &env->img[4], 1, env);
	process_image(&env->sdl, &env->img[5], 3, env);
	process_image(&env->sdl, &env->img[1], 2, env);
	process_image(&env->sdl, &env->img[2], 2, env);
	if (env->sidebar == 1)
		process_image(&env->sdl, &env->img[3], 2, env);
	SDL_UpdateWindowSurface(env->sdl.window);
}
