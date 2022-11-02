/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:43:48 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/02 16:17:28 by ctrouve          ###   ########.fr       */
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
		close_prog(NULL, "Creating window surface failed...", -1);
}


int	main(int argc, char **argv)
{
	t_env	env;
	t_2i	mouse_coords;

	close_prog(&env, "Initializing close program function.", 42);
	ft_bzero(&env, sizeof(t_env));
	env.scene = malloc(sizeof(t_scene));
	if (env.scene == NULL)
		close_prog(NULL, "Malloc env.scene failed...", -1);
	env.scene->camera = load_scene_camera(argv[1]);
	env.scene->objects_list = load_scene_objects(argv[1]);
	env.scene->lights_list = load_scene_lights(argv[1]);
	sdl_init(&env.sdl);
	env.img = create_images(IMAGES);
	if (env.img == NULL)
		close_prog(NULL, "Creating images failed...", -1);
	env.mouse_state = 0;
	while (1)
	{
		if (env.sdl.event.type == SDL_QUIT || env.sdl.event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
		{
			SDL_Quit();
			break ;
		}
		if (env.sdl.event.type == SDL_MOUSEMOTION)
			mouse_move(&env);
		SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y);
		if (env.sdl.event.type == SDL_MOUSEBUTTONUP)
		{
			if (env.sdl.event.button.button == SDL_BUTTON_LEFT)
				left_button_up(mouse_coords, &env);
			if (env.sdl.event.button.button == SDL_BUTTON_RIGHT)
				right_button_up(mouse_coords, &env);
		}
		if (env.sdl.event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (env.sdl.event.button.button == SDL_BUTTON_LEFT)
				left_button_down(mouse_coords, &env);
			if (env.sdl.event.button.button == SDL_BUTTON_RIGHT)
				right_button_down(mouse_coords, &env);
		}
		if (env.sdl.event.type == SDL_MOUSEBUTTONUP)
		{
			process_image(&env.sdl, &env.img[0], 1, &env);
			process_image(&env.sdl, &env.img[1], 1, &env);
			process_image(&env.sdl, &env.img[2], 1, &env);
		}
		// printf("MOUSE STATE %d\n", env.mouse_state);
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
