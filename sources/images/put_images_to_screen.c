/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_images_to_screen.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:05:29 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/04 21:03:12 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	put_images_to_screen(t_env *env)
{
	if (env->scene->resolution.x == env->scene->resolution_range.x && env->scene->resolution.y == env->scene->resolution_range.x)
		process_image(&env->sdl, &env->img[0], 2, env);
	else
		process_image(&env->sdl, &env->img[0], 3, env);
	process_image(&env->sdl, &env->img[1], 2, env);
	process_image(&env->sdl, &env->img[2], 2, env);
	if (env->sidebar == 1)
		process_image(&env->sdl, &env->img[3], 2, env);
	SDL_UpdateWindowSurface(env->sdl.window);
}
