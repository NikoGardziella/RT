/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:43:48 by dmalesev          #+#    #+#             */
/*   Updated: 2022/10/31 15:42:30 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	close_prog(void *param, char *exit_msg, int exit_code)
{
	static t_env	*env;

	if (env == NULL)
	{
		env = param;
		return ;
	}
	free_images(env->img, IMAGES);
	//ft_lstdel(&env->scene, &del_object);
	ft_putendl(exit_msg);
	exit (exit_code);
}

void	sdl_init(t_sdl *sdl)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_Quit();
	if (SCREEN_X < 200 || SCREEN_X > 2560 || SCREEN_Y < 200 || SCREEN_Y > 1440)
		close_prog(NULL, "Window size specified not in range...", -1);
	sdl->window = SDL_CreateWindow("Ray Tracer", 0, 0, SCREEN_X, SCREEN_Y, 0);
	if (sdl->window == NULL)
		close_prog(NULL, "Creating window failed...", -1);
	sdl->screen = SDL_GetWindowSurface(sdl->window);
	if (sdl->screen == NULL)
		close_prog(NULL, "Creting window surface failed...", -1);
}

void	put_image_to_screen_surface(t_img *img, SDL_Surface *screen)
{
	uint8_t	*dest;
	int		offset;
	int		y;

	y = 0;
	offset = img->dim.start.y * screen->pitch;
	offset += img->dim.start.x * screen->format->BytesPerPixel;
	while (y < img->dim.size.y)
	{
		dest = ((uint8_t *)screen->pixels + offset + (screen->pitch * y));
		dest = (uint8_t *)ft_memcpy(dest, img->surface->pixels + img->surface->pitch * y, (size_t)img->surface->pitch);
		y += 1;
	}
}

/*CONTINUE MAKING OWN BLIT FUNCTION*/
void	blit_surface(SDL_Surface *src, t_dim srcrect, SDL_Surface *dest, t_dim destrect)
{
	uint8_t	*srcaddr;
	uint8_t	*destaddr;
	uint32_t	*addr;
	uint32_t	*start;
	t_2f	coords;
	int		offset;
	float	step;

	offset = destrect.start.y * dest->pitch;
	offset += destrect.start.x * dest->format->BytesPerPixel;
	coords.y = 0;
	printf("SOURCES BPP! %d\n", src->format->BytesPerPixel);
	printf("DEST BPP! %d\n", dest->format->BytesPerPixel);
	printf("SOURCES SIZE! %d %d\n", src->w, src->h);
	printf("DEST SIZE! %d %d\n", dest->w, dest->h);
	step = (float)srcrect.size.x / (float)destrect.size.x;
	printf("STEP! %f\n", step);
	while ((int)(coords.y * step) < srcrect.size.y)
	{
		destaddr = ((uint8_t *)dest->pixels + offset + (dest->pitch * (int)coords.y));
		srcaddr = ((uint8_t *)src->pixels + (src->pitch * (int)coords.y) + (srcrect.start.x * src->format->BytesPerPixel) + (srcrect.start.y * src->pitch));
		/*if (srcaddr >= (uint8_t *)src->pixels && srcaddr < (uint8_t *)src->pixels + (srcrect.size.x * src->format->BytesPerPixel) + (srcrect.size.y * src->pitch))
			srcaddr = (uint8_t *)ft_memcpy(destaddr, srcaddr, (size_t)srcrect.size.x * (src->format->BytesPerPixel));*/
		coords.x = 0;
		while ((int)coords.x * step < srcrect.size.x)
		{
			printf("TEST! %f\n", step);
			offset = (int)(coords.y / step) * dest->pitch;
			offset += (int)(coords.x / step) * dest->format->BytesPerPixel;

			offset += destrect.start.x * dest->format->BytesPerPixel + destrect.start.y * dest->pitch;
			destaddr = (uint8_t *)(dest->pixels + offset);

			offset = (int)(coords.y * step) * src->pitch;
			offset += (int)(coords.x * step) * src->format->BytesPerPixel;

			offset += srcrect.start.x * src->format->BytesPerPixel + srcrect.start.y * src->pitch;
			srcaddr = (uint8_t *)(src->pixels + offset);
			addr = (uint32_t *)(dest->pixels + offset);
			//if (((uint32_t *)srcaddr - (uint32_t *)addr) < ((uint32_t *)addr + (uint32_t)(dest->h * dest->w)) && (uint32_t *)srcaddr - (uint32_t *)addr > (uint32_t *)addr + (uint32_t)(dest->h * dest->w))

			uint32_t	*(limit[3]);
			limit[0] = dest->pixels;
			start = limit[0];
			limit[0] -= start;
			limit[1] = (uint32_t *)dest->pixels + (dest->w * dest->h) - start;
			limit[2] = (uint32_t *)srcaddr - start;


			printf("H: %d W: %d LIMIT BOUND START: %lu BOUND END: %lu PTR: %lu\n", dest->h, dest->w, (size_t)limit[0], (size_t)limit[1], (size_t)limit[2]);
			//if (limit[2] - start > limit[1] - start && limit[2] - start < limit[0] - start)
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
		put_image_to_screen_surface(img, sdl->screen);
		SDL_UpdateWindowSurface(sdl->window);
		(void)sdl;
	}
}

int	main(int argc, char **argv)
{
	t_env		env;
	SDL_Surface	*test;
	SDL_Rect	rect[2];
	t_dim		dim[2];

	close_prog(&env, "Initializing close program function.", 42);
	ft_bzero(&env, sizeof(t_env));
	load_scene_objects(argv[1]);
	sdl_init(&env.sdl);
	env.img = create_images(IMAGES);
	if (env.img == NULL)
		close_prog(NULL, "Creating images failed...", -1);
	/*DELETE THIS LATER...*/
	glob_img = &env.img[0];
	/*THIS*/
	process_image(&env.sdl, &env.img[0], 1, &env);
	process_image(&env.sdl, &env.img[1], 1, &env);
	test = SDL_LoadBMP("test.bmp");
	test = SDL_ConvertSurfaceFormat(test, SDL_PIXELFORMAT_ARGB8888, 0);
	rect[0] = (SDL_Rect){300, 0, 200, 200};
	rect[1] = (SDL_Rect){300, 320, 200, 200};
	dim[0].start = (t_2i){0, 0};
	dim[0].size = (t_2i){200, 200};
	dim[1].start = (t_2i){250, 250};
	dim[1].size = (t_2i){100, 100};
	SDL_BlitSurface(test, NULL, env.sdl.screen, &rect[0]);
	blit_surface(test, dim[0], env.sdl.screen, dim[1]);
	SDL_UpdateWindowSurface(env.sdl.window);
	while (1)
	{
		if (env.sdl.event.type == SDL_QUIT || env.sdl.event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			SDL_Quit();
			break ;
		}
		/*else if (env.sdl.event.type == SDL_WINDOWEVENT)
		{
			if (env.sdl.event.window.event == SDL_WINDOWEVENT_EXPOSED)
				put_images_to_screen(env);
		}*/
		/*Find out what this does!*/
		SDL_WaitEvent(&env.sdl.event);
	}
	SDL_DestroyWindow(env.sdl.window);
	(void)argc;
	(void)argv;
	return(0);
}
