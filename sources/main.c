/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:43:48 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/01 15:05:44 by dmalesev         ###   ########.fr       */
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
	// free_images(env->img, IMAGES);
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


int	main(int argc, char **argv)
{
	t_env		env;
	SDL_Surface	*test;
	t_dim		dim[2];
	t_2i		mouse_coords;

	close_prog(&env, "Initializing close program function.", 42);
	ft_bzero(&env, sizeof(t_env));
	load_scene_objects(argv[1]);
	sdl_init(&env.sdl);
	env.img = create_images(IMAGES);
	if (env.img == NULL)
		close_prog(NULL, "Creating images failed...", -1);

	//process_image(&env.sdl, &env.img[0], 1, &env);
	process_image(&env.sdl, &env.img[1], 1, &env);
	test = SDL_LoadBMP("test.bmp");
	//test = SDL_ConvertSurfaceFormat(test, SDL_PIXELFORMAT_ARGB8888, 0);
	//env.scene->objects = load_scene_objects(argv[1]);
	dim[0].start = (t_2i){0, 0};
	dim[0].size = (t_2i){1000, 1000};

	dim[1].start = (t_2i){0, 0};
	dim[1].size = (t_2i){500, 500};

	while (1)
	{
		if (env.sdl.event.type == SDL_QUIT || env.sdl.event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			SDL_Quit();
			break ;
		}
		if (SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y) == 1)
		{
			dim[1].start = mouse_coords;
			blit_surface(test, dim[0], env.sdl.screen, dim[1]);
			SDL_UpdateWindowSurface(env.sdl.window);
		}
		if (env.sdl.event.type == SDL_MOUSEWHEEL)
		{
			if (env.sdl.event.wheel.y != 0) // scroll up
			{
				dim[1].size.x += env.sdl.event.wheel.y;
				dim[1].size.y += env.sdl.event.wheel.y;
			}
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
