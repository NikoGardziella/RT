/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_image.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:21:22 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/20 13:58:08 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	process_image(t_sdl *sdl, t_img *img, int mode, void *param)
{
	SDL_LockSurface(sdl->screen);
	if ((mode & 1) == 1)
		ft_bzero(img->surface->pixels,
			(size_t)(img->surface->h * img->surface->pitch));
	if (img->draw_func != NULL && (mode & 8) != 8)
		img->draw_func(img, param);
	if ((mode & 2) == 2)
		blit_surface(img->surface, NULL, sdl->screen, &img->dim);
	if ((mode & 4) == 4)
	{
		SDL_UpdateWindowSurface(sdl->window);
	}
	SDL_UnlockSurface(sdl->screen);
}
