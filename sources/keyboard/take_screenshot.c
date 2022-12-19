/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_screenshot.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:15:40 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/19 13:15:48 by ngardzie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	take_screenshot(t_env *env)
{
	SDL_Surface	*sshot;

	sshot = SDL_CreateRGBSurface(0, SCREEN_X, SCREEN_Y, 24,
			0x00FF0000, 0x0000FF00, 0x000000FF, 0);
	blit_surface(env->img[0].surface, NULL, sshot, NULL);
	SDL_LockSurface(sshot);
	if (SDL_SaveBMP(sshot, "screenshot.bmp") != 0)
		ft_putendl("SDL screenshot error");
	SDL_UnlockSurface(sshot);
	SDL_FreeSurface(sshot);
}
