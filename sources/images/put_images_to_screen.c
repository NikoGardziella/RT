/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_images_to_screen.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:05:29 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/04 12:39:46 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	time_since_success(double ammount, int id)
{
	static struct timespec	time[8];
	struct timespec			curr_time;
	long int				elapsed;

	clock_gettime(CLOCK_MONOTONIC, &curr_time);
	elapsed = (curr_time.tv_sec - time[id].tv_sec) * 1000000
		+ (curr_time.tv_nsec - time[id].tv_nsec) / 1000;
	if (elapsed > (long int)(ammount * 1000000.0))
	{
		time[id] = curr_time;
	}
	return ((double)elapsed / 1000000.0);
}

void	put_images_to_screen(t_env *env)
{
	double	plot_time;

	if (env->scene->resolution.x == env->scene->resolution_range.x && env->scene->resolution.y == env->scene->resolution_range.x)
		process_image(&env->sdl, &env->img[0], 2, env);
	else
		process_image(&env->sdl, &env->img[0], 3, env);
	process_image(&env->sdl, &env->img[1], 2, env);
	process_image(&env->sdl, &env->img[2], 2, env);
	if (env->sidebar == 1)
		process_image(&env->sdl, &env->img[3], 2, env);
	SDL_UpdateWindowSurface(env->sdl.window);
	plot_time = (double)time_since_success(0.0f, 0);
	// printf("Plot time %f\n", plot_time);
}
