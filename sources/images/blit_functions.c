/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   blit_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 18:13:25 by pnoutere          #+#    #+#             */
/*   Updated: 2022/12/19 18:36:33 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_dim	get_dimensions(SDL_Surface *src)
{
	t_dim	dimensions;

	dimensions.size = (t_2i){src->w, src->h};
	dimensions.start = (t_2i){0, 0};
	return (dimensions);
}

void	blit_init(t_blit *b)
{
	b->srcrect->size.x = ft_max(b->srcrect->size.x, 1);
	b->srcrect->size.y = ft_max(b->srcrect->size.y, 1);
	b->srcrect->size.x = ft_min(b->srcrect->size.x, b->src->w);
	b->srcrect->size.y = ft_min(b->srcrect->size.y, b->src->h);
	b->srcrect->start.x = ft_max(b->srcrect->start.x, 0);
	b->srcrect->start.y = ft_max(b->srcrect->start.y, 0);
	b->srcrect->start.x = ft_min(b->srcrect->start.x, b->src->w);
	b->srcrect->start.y = ft_min(b->srcrect->start.y, b->src->h);
	b->destrect->size.x = ft_max(b->destrect->size.x, 1);
	b->destrect->size.y = ft_max(b->destrect->size.y, 1);
	b->step.x = (double)b->srcrect->size.x / (double)b->destrect->size.x;
	b->step.y = (double)b->srcrect->size.y / (double)b->destrect->size.y;
	b->coords.y = 0;
}

void	blit_loop_functionality(t_blit b, SDL_Surface *src, SDL_Surface *dest)
{
	b.offset = (uint32_t)((int)((b.coords.y + b.srcrect->start.y)
				* b.step.y) *(src->pitch));
	b.offset += (uint32_t)((int)((b.coords.x + b.srcrect->start.x)
				* b.step.x) *(src->format->BytesPerPixel));
	b.srcaddr = (src->pixels + b.offset);
	b.offset = (uint32_t)((b.coords.y
				+ b.destrect->start.y) * dest->pitch);
	b.offset += (uint32_t)((b.coords.x
				+ b.destrect->start.x) * dest->format->BytesPerPixel);
	b.destaddr = (dest->pixels + b.offset);
	if (b.destaddr - (uint32_t *)dest->pixels >= 0x0
		&& b.destaddr - (uint32_t *)dest->pixels < dest->w * dest->h)
	{
		if (b.srcaddr - (uint32_t *)src->pixels >= 0x0
			&& b.srcaddr - (uint32_t *)src->pixels < src->w * src->h)
		{
			if (((int)b.coords.x + (int)b.destrect->start.x >= 0))
				*b.destaddr = *b.srcaddr;
		}
	}
}

void	blit_loop(t_blit b, SDL_Surface *src, SDL_Surface *dest)
{	
	while ((double)((b.coords.y + b.srcrect->start.y) * b.step.y)
		< (double)b.srcrect->size.y)
	{
		b.coords.x = 0;
		while ((double)((b.coords.x + b.srcrect->start.x)
			* b.step.x) < (double)b.srcrect->size.x)
		{
			if (((int)b.coords.x + (int)b.destrect->start.x >= (int)(dest->w)))
				break ;
			blit_loop_functionality(b, src, dest);
			b.coords.x += 1;
		}
		b.coords.y += 1;
	}
}

void	blit_surface(SDL_Surface *src, t_dim *srcrect,
	SDL_Surface *dest, t_dim *destrect)
{	
	t_blit		b;
	t_dim		dim[2];

	b.src = src;
	b.dest = dest;
	if (src == NULL || dest == NULL)
		return ;
	if (srcrect == NULL)
	{
		dim[0] = (t_dim){(t_2i){src->w, src->h}, (t_2i){0, 0}};
		srcrect = &dim[0];
	}
	if (destrect == NULL)
	{
		dim[1] = (t_dim){(t_2i){dest->w, dest->h}, (t_2i){0, 0}};
		destrect = &dim[1];
	}
	b.srcrect = srcrect;
	b.destrect = destrect;
	blit_init(&b);
	blit_loop(b, src, dest);
}
