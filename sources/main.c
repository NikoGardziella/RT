/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:43:48 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/20 15:12:55 by pnoutere         ###   ########.fr       */
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
	if (exit_code == -19)
		exit(exit_code);
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

void	render_screen(t_env *env)
{
	double	fps;

	fps = 0.0001;
	if (time_since_success(fps, 0, 2) >= fps)
	{
		env->frame_index = 0;
		env->scene->subframe.x = env->scene->subframe_range.x;
		env->scene->subframe.y = env->scene->subframe_range.x;
		put_images_to_screen(env);
	}
}

void	main_checks(int argc, char **argv, t_env *env)
{
	close_prog(env, "Initializing close program function.", 42);
	if (argc > 2)
		close_prog(NULL, "Too many arguments to run program...", -19);
	ft_bzero(env, sizeof(t_env));
	env->file_path = argv[1];
	init_main(env);
	printf("here 1\n");
	env->scene->camera = load_scene_camera(argv[1]);
	if (env->scene->camera == NULL)
		close_prog(NULL, "Parsing camera failed...", -1);
	env->camera_default = env->scene->camera->ray.forward;
	env->scene->object_list = load_scene_objects(argv[1]);
	if (env->scene->object_list == NULL)
		close_prog(NULL, "Parsing objects failed...", -1);
	env->scene->light_list = load_scene_lights(argv[1]);
	if (env->scene->light_list == NULL)
		close_prog(NULL, "Parsing lights failed...", -1);
	env->scene->camera_angle = (t_3d){0.0f, 0.0f, 0.0f};
	sdl_init(&env->sdl);
	SDL_RaiseWindow(env->sdl.window);
	env->img = create_images(IMAGES);
	if (env->img == NULL)
		close_prog(NULL, "Creating images failed...", -1);
	env->mouse.state = 0;
	env->keymap = 0;
}

void	poll_event(t_env *env, int *running)
{
	while (SDL_PollEvent(&env->sdl.event))
	{
		if (env->sdl.event.type == SDL_QUIT)
			*running = 0;
		mouse_events(env);
		keyboard_events(env);
		if (env->sdl.event.type == SDL_WINDOWEVENT)
		{
			if (env->sdl.event.window.event == SDL_WINDOWEVENT_EXPOSED)
				put_images_to_screen(env);
		}
	}
}

int	main(int argc, char **argv)
{
	t_env	env;
	int		running;

	main_checks(argc, argv, &env);
	running = 1;
	while (running == 1)
	{
		poll_event(&env, &running);
		if (running == 0)
			break ;
		if (time_since_success(0.01, 1, 2) >= 0.01)
			continue ;
		if (((keyboard_hold(&env) & 1) == 1) | ((mouse_main(&env) & 1) == 1))
			render_screen(&env);
		prog_clock(&env);
		put_images_to_screen(&env);
	}
	close_prog(NULL, "Exited successfully using [X].", 0);
	return (0);
}
