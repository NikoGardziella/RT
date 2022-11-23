/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:43:48 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/18 11:57:24 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	del_object(void *content, size_t content_size)
{
	t_object	*object;

	object = (t_object *)content;
	if (object != NULL)
	{
		ft_bzero(content, content_size);
		free(object);
	}
}

void	close_prog(void *param, char *exit_msg, int exit_code)
{
	static t_env	*env;

	if (env == NULL)
	{
		env = param;
		return ;
	}
	free_images(env->img, IMAGES);
	if (env->font != NULL)
		free_font(&env->font);
	if (env->scene->object_list != NULL)
		ft_lstdel(&env->scene->object_list, &del_object);
	if (env->scene->light_list != NULL)
		ft_lstdel(&env->scene->light_list, &del_object);
	if (env->scene->camera != NULL)
		free(env->scene->camera);
	if (env->sdl.screen != NULL)
		SDL_FreeSurface(env->sdl.screen);
	if (env->sdl.window != NULL)
		SDL_DestroyWindow(env->sdl.window);
	SDL_Quit();
	ft_putendl(exit_msg);
	exit (exit_code);
}

void	sdl_init(t_sdl *sdl)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_Quit();
	if (SCREEN_X < 200 || SCREEN_X > 2560 || SCREEN_Y < 200 || SCREEN_Y > 1440)
		close_prog(NULL, "Window size specified not in range...", -1);
	sdl->window = SDL_CreateWindow("Ray Tracer", 800, 400, SCREEN_X, SCREEN_Y, 0);
	if (sdl->window == NULL)
		close_prog(NULL, "Creating window failed...", -1);
	sdl->screen = SDL_GetWindowSurface(sdl->window);
	if (sdl->screen == NULL)
		close_prog(NULL, "Creating window surface failed...", -1);
}

void	render_screen(t_env *env)
{
	double	fps;

	fps = 0.0001;
	if (time_since_success(fps, 0) >= fps)
	{
		env->scene->resolution.x = env->scene->resolution_range.x;
		env->scene->resolution.y = env->scene->resolution_range.x;
		put_images_to_screen(env);
	}
}

int	main(int argc, char **argv)
{
	t_env	env;
	int		running;

	close_prog(&env, "Initializing close program function.", 42);
	if (argc > 2)
		close_prog(NULL, "Too many arguments to run program...", -1);
	ft_bzero(&env, sizeof(t_env));
	init_main(&env);
	env.scene->camera = load_scene_camera(argv[1]);
	if (env.scene->camera == NULL)
		close_prog(NULL, "Parsing camera failed...", -1);
	env.scene->object_list = load_scene_objects(argv[1]);
	if (env.scene->object_list == NULL)
		close_prog(NULL, "Parsing objects failed...", -1);
	env.scene->light_list = load_scene_lights(argv[1]);
	if (env.scene->light_list == NULL)
		close_prog(NULL, "Parsing lights failed...", -1);
	env.scene->camera_angle = (t_3d){0.0f, 0.0f, 0.0f};
	sdl_init(&env.sdl);
	SDL_RaiseWindow(env.sdl.window);
	env.img = create_images(IMAGES);
	if (env.img == NULL)
		close_prog(NULL, "Creating images failed...", -1);
	env.mouse.state = 0;
	env.keymap = 0;
	running = 1;
	while (running == 1)
	{
		while (SDL_PollEvent(&env.sdl.event))
		{
			if (env.sdl.event.type == SDL_QUIT)
			{
				running = 0;
			}
			mouse_events(&env);
			keyboard_events(&env);
			if (env.sdl.event.type == SDL_WINDOWEVENT)
			{
				if (env.sdl.event.window.event == SDL_WINDOWEVENT_EXPOSED)
					put_images_to_screen(&env);
			}
		}
		if (running == 0)
			break ;
		if (time_since_success(0.01, 1) >= 0.01)
			continue ;
		if (((keyboard_hold(&env) & 1) == 1) | ((mouse_main(&env) & 1) == 1))
			render_screen(&env);
		put_images_to_screen(&env);
	}
	close_prog(NULL, "Exited successfully using [X].", 0);
	return(0);
}
