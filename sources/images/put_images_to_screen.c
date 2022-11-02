/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_images_to_screen.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 15:05:29 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/02 15:09:18 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	put_images_to_screen(t_env *env)
{
	blit_surface(env->img[0].surface, NULL, env->sdl.screen, &env->img[0].dim);
	blit_surface(env->img[1].surface, NULL, env->sdl.screen, &env->img[1].dim);
	blit_surface(env->img[2].surface, NULL, env->sdl.screen, &env->img[2].dim);
	SDL_UpdateWindowSurface(env->sdl.window);
}
