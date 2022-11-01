/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_image.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:21:22 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/01 12:28:01 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	blit_surface(SDL_Surface *src, t_dim srcrect, SDL_Surface *dest, t_dim destrect)
{
	uint32_t	offset;
	uint32_t	*srcaddr;
	uint32_t	*destaddr;
	t_2f		step;
	t_2f		coords;

	if (src == NULL || dest == NULL)
		return ;
	srcrect.size.x = ft_min(srcrect.size.x, src->w);
	srcrect.size.y = ft_min(srcrect.size.y, src->h);
	destrect.size.x = ft_max(destrect.size.x, 1);
	destrect.size.y = ft_max(destrect.size.y, 1);
	printf("SOURCES BPP! %d\n", src->format->BytesPerPixel);
	printf("DEST BPP! %d\n", dest->format->BytesPerPixel);
	printf("SOURCES SIZE! %d %d\n", src->w, src->h);
	printf("SRCRECT SIZE! %d %d\n", srcrect.size.x, srcrect.size.y);
	printf("DEST SIZE! %d %d\n", dest->w, dest->h);
	printf("DESTRECT SIZE! %d %d\n", destrect.size.x, destrect.size.y);
	step.x = (float)srcrect.size.x / (float)destrect.size.x;
	step.y = (float)srcrect.size.y / (float)destrect.size.y;
	printf("STEP.X %f STEP.Y %f\n", step.x, step.y);
	coords.y = 0;
	while ((coords.y * step.y) < srcrect.size.y)
	{
		coords.x = 0;
		while ((coords.x * step.x) < srcrect.size.x)
		{
			offset = (uint32_t)(coords.y * dest->pitch);
			offset += (uint32_t)(coords.x * dest->format->BytesPerPixel);
			offset += (uint32_t)(destrect.start.x * dest->format->BytesPerPixel + destrect.start.y * dest->pitch);
			destaddr = (dest->pixels + offset);

			offset = (uint32_t)((int)(coords.y * step.y) * src->pitch);
			offset += (uint32_t)((int)(coords.x * step.x) * src->format->BytesPerPixel);
			offset += (uint32_t)(srcrect.start.x * src->format->BytesPerPixel + srcrect.start.y * src->pitch);
			srcaddr = (src->pixels + offset);

			if (destaddr - (uint32_t *)dest->pixels >= 0x0 && destaddr - (uint32_t *)dest->pixels < dest->w * dest->h)
				*destaddr = *srcaddr;
			coords.x += 1;
		}
		coords.y += 1;
	}
}

void	process_image(t_sdl *sdl, t_img *img, int mode, void *param)
{
	fill_image(img, 0x00000000);
	if (img->draw_func != NULL)
		img->draw_func(img, param);
	if (mode == 1)
	{
		//blit_surface(img->surface, img->dim, sdl->screen, img->dim);
		SDL_UpdateWindowSurface(sdl->window);
		(void)sdl;
	}
}
