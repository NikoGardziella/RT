/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:43:48 by dmalesev          #+#    #+#             */
/*   Updated: 2022/10/31 11:51:05 by dmalesev         ###   ########.fr       */
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
	uint8_t	*addr;
	t_2i	coords;
	int		offset;

	offset = destrect.start.y * dest->pitch;
	offset += destrect.start.x * dest->format->BytesPerPixel;
	coords.y = 0;
	while (coords.y < srcrect.size.y)
	{
		addr = ((uint8_t *)dest->pixels + offset + (dest->pitch * coords.y));
		if (addr >= (uint8_t *)dest->pixels)
			addr = (uint8_t *)ft_memcpy(addr, src->pixels + src->pitch * coords.y, (size_t)destrect.size.x * src->format->BytesPerPixel);
		else
			printf("HERE! %d\n", coords.y);
		coords.x = 0;
		/*while (coords.x < srcrect.size.x)
		{
			//offset = src->pitch;
			addr = ((uint8_t *)dest->pixels + offset + (uint8_t)(coords.x) + (dest->pitch * coords.y));
			*addr = *(uint8_t *)src->pixels + (uint8_t)(coords.x) + (uint8_t)(src->pitch * coords.y);
			//addr += (uint8_t)src->format->BytesPerPixel;
			coords.x += 1;
		}*/
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
	sdl_init(&env.sdl);
	env.img = create_images(IMAGES);
	if (env.img == NULL)
		close_prog(NULL, "Creating images failed...", -1);
	process_image(&env.sdl, &env.img[0], 1, &env);
	process_image(&env.sdl, &env.img[1], 1, &env);
	test = SDL_LoadBMP("test.bmp");
	rect[0] = (SDL_Rect){300, 0, 200, 200};
	rect[1] = (SDL_Rect){300, 320, 200, 200};
	dim[0].start = (t_2i){0, 100};
	dim[0].size = (t_2i){100, 100};
	dim[1].start = (t_2i){0, 0};
	dim[1].size = (t_2i){200, 200};
	SDL_BlitSurface(test, NULL, env.sdl.screen, &rect[0]);
	blit_surface(test, dim[1], env.sdl.screen, dim[0]);
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
