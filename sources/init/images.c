/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   images.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 10:56:32 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/23 15:11:33 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	get_image_sizes(t_img *img)
{
	t_2i	button;
	t_2i	offset;
	t_2i	slider;

	button = (t_2i){SCREEN_X / 40, SCREEN_Y / 25};
	offset = (t_2i){button.x * 30 / 100, button.y * 30 / 100};
	img[0].dim.size = (t_2i){SCREEN_X, SCREEN_Y};
	img[1].dim.size = (t_2i){button.x, button.y};
	img[2].dim.size = (t_2i){SCREEN_X / 4, SCREEN_Y / 4};
	img[3].dim.size = (t_2i){SCREEN_X / 4, SCREEN_Y - (offset.y * 2)};
	slider = (t_2i){img[3].dim.size.x - offset.x * 2, button.y};
	img[4].dim.size = (t_2i){SCREEN_X, SCREEN_Y};
	img[5].dim.size = (t_2i){SCREEN_X, SCREEN_Y};
	img[6].dim.size = slider;
	img[7].dim.size = slider;
}

static void	get_image_positions(t_img *img)
{
	t_2i	button;
	t_2i	offset;

	button = (t_2i){SCREEN_X / 40, SCREEN_Y / 25};
	offset = (t_2i){button.x * 30 / 100, button.y * 30 / 100};
	img[0].dim.start = (t_2i){0, 0};
	img[1].dim.start = (t_2i){0 + offset.x, 0 + offset.y};
	img[2].dim.start = (t_2i){SCREEN_X - img[2].dim.size.x - offset.x, 0 + offset.y};
	img[3].dim.start = (t_2i){0 + offset.x, 0 + offset.y};
	img[4].dim.start = (t_2i){0, 0};
	img[5].dim.start = (t_2i){0, 0};
	img[6].dim.start = (t_2i){img[3].dim.start.x + offset.x,img[3].dim.start.y + img[3].dim.size.y / 2};
	img[7].dim.start = (t_2i){img[3].dim.start.x + offset.x,img[3].dim.start.y + img[3].dim.size.y / 3};
}

static void	get_image_functions(t_img *img)
{
	img[0].draw_func = &main_image;
	img[1].draw_func = &sidebar_button;
	img[2].draw_func = &ray_debugger;
	img[3].draw_func = &sidebar;
	img[4].draw_func = NULL;
	img[5].draw_func = &gradual_render;
	img[6].draw_func = &slider;
	img[7].draw_func = &slider;
}

t_img	*free_images(t_img *img, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (img && img[i].surface != NULL)
			SDL_FreeSurface(img[i].surface);
		i++;
	}
	if (img != NULL)
		free(img);
	return (NULL);
}

t_img	*create_images(size_t count)
{
	t_img	*img;
	size_t	i;

	img = (t_img *)malloc(sizeof(t_img) * count);
	if (img == NULL)
		return (NULL);
	ft_bzero(img, sizeof(t_img));
	i = 0;
	get_image_sizes(img);
	get_image_positions(img);
	while (i < count)
	{
		if (img[i].dim.size.x <= 0 || img[i].dim.size.y <= 0)
			return (free_images(img, i));
		img[i].surface = SDL_CreateRGBSurface(0,
				img[i].dim.size.x, img[i].dim.size.y, 32,
				0xFF0000, 0x00FF00, 0x0000FF, 0xFF000000);
		if (img[i].surface == NULL)
			return (free_images(img, count));
		i++;
	}
	get_image_functions(img);
	return (img);
}
