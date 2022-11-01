/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_image.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/01 11:21:22 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/01 11:34:59 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	blit_surface(SDL_Surface *src, t_dim srcrect, SDL_Surface *dest, t_dim destrect)
{
	uint32_t	*addr;
	uint8_t		*srcaddr;
	uint8_t		*destaddr;
	t_2f		step;
	t_2f		coords;
	int			offset;

	offset = destrect.start.y * dest->pitch;
	offset += destrect.start.x * dest->format->BytesPerPixel;
	coords.y = 0;
	printf("SOURCES BPP! %d\n", src->format->BytesPerPixel);
	printf("DEST BPP! %d\n", dest->format->BytesPerPixel);
	printf("SOURCES SIZE! %d %d\n", src->w, src->h);
	printf("DEST SIZE! %d %d\n", dest->w, dest->h);
	step.x = (float)srcrect.size.x / (float)destrect.size.x;
	step.y = (float)srcrect.size.y / (float)destrect.size.y;
	printf("STEP.X %f STEP.Y %f\n", step.x, step.y);
	while ((coords.y * step.y) < srcrect.size.y)
	{
		destaddr = ((uint8_t *)dest->pixels + offset + (dest->pitch * (int)coords.y));
		srcaddr = ((uint8_t *)src->pixels + (src->pitch * (int)coords.y) + (srcrect.start.x * src->format->BytesPerPixel) + (srcrect.start.y * src->pitch));
		coords.x = 0;
		while ((coords.x * step.x) < srcrect.size.x)
		{
			offset = (int)(coords.y) * dest->pitch;
			offset += (int)(coords.x) * dest->format->BytesPerPixel;
			offset += destrect.start.x * dest->format->BytesPerPixel + destrect.start.y * dest->pitch;
			destaddr = (uint8_t *)(dest->pixels + offset);

			offset = (int)(coords.y * step.y) * src->pitch;
			offset += (int)(coords.x * step.x) * src->format->BytesPerPixel;
			offset += srcrect.start.x * src->format->BytesPerPixel + srcrect.start.y * src->pitch;
			srcaddr = (uint8_t *)(src->pixels + offset);

			addr = (uint32_t *)(destaddr);

			if (addr - (uint32_t *)dest->pixels >= 0x0 && addr - (uint32_t *)dest->pixels < dest->w * dest->h)
				*addr = *(uint32_t *)srcaddr;
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
