/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:07:49 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/20 09:59:31 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	sdl_init(t_sdl *sdl)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_Quit();
	if (SCREEN_X < 200 || SCREEN_X > 2560 || SCREEN_Y < 200 || SCREEN_Y > 1440)
		close_prog(NULL, "Window size specified not in range...", -1);
	sdl->window = SDL_CreateWindow("Ray Tracer",
			800, 400, SCREEN_X, SCREEN_Y, 0);
	if (sdl->window == NULL)
		close_prog(NULL, "Creating window failed...", -1);
	sdl->screen = SDL_GetWindowSurface(sdl->window);
	if (sdl->screen == NULL)
		close_prog(NULL, "Creating window surface failed...", -1);
}

static void	load_textures(t_env *env)
{
	env->bmptxtr.sidebar = SDL_LoadBMP("ui_sidebar.bmp");
	if (env->bmptxtr.sidebar == NULL)
		close_prog(NULL, "Failed to load BMP texture...", -3);
	env->bmptxtr.slider = SDL_LoadBMP("ui_slider.bmp");
	if (env->bmptxtr.slider == NULL)
		close_prog(NULL, "Failed to load BMP texture...", -3);
	env->bmptxtr.slide = SDL_LoadBMP("ui_slide.bmp");
	if (env->bmptxtr.slide == NULL)
		close_prog(NULL, "Failed to load BMP texture...", -3);
}

void	init_checks(t_env *env)
{
	if (env->scene->accum_buffer == NULL)
		close_prog(NULL, "Failed to malloc for accum_buffer...", -4);
	env->scene->cam_hit_buffer = (t_cam_hit *)malloc(
			sizeof(t_cam_hit) * (SCREEN_X * SCREEN_Y));
	if (env->scene->cam_hit_buffer == NULL)
		close_prog(NULL, "Failed to malloc for cam_hit_buffer...", -5);
	env->scene->cam_hit_color = (uint32_t *)malloc(sizeof(uint32_t)
			* (SCREEN_X * SCREEN_Y));
	if (env->scene->cam_hit_color == NULL)
		close_prog(NULL, "Failed to malloc for cam_hit_color...", -5);
	env->scene->cam_hit_intensity = (uint32_t *)malloc(sizeof(uint32_t)
			* (SCREEN_X * SCREEN_Y));
	if (env->scene->cam_hit_intensity == NULL)
		close_prog(NULL, "Failed to malloc for cam_hit_intensity...", -5);
}

void	init_main(t_env *env)
{
	char	*font_path;

	env->scene = ft_memalloc(sizeof(t_scene));
	if (env->scene == NULL)
		close_prog(NULL, "Malloc env.scene failed...", -1);
	font_path = "libraries/dm_bdf_render/examples/bdf_files/ic8x8u.bdf";
	font_path = "cascadia_code_semi_bold-12.bdf";
	env->font = load_font(font_path);
	if (env->font == NULL)
		close_prog(NULL, "Failed to load font...", -2);
	env->scene->resolution_range = (t_2i){0, 3};
	env->scene->resolution.x = env->scene->resolution_range.x;
	env->scene->resolution.y = env->scene->resolution_range.x;
	env->scene->accum_resolution.x = env->scene->resolution_range.x;
	env->scene->accum_resolution.y = env->scene->resolution_range.x;
	env->scene->accum_buffer = (t_3d *)malloc(sizeof(t_3d)
			* (SCREEN_X * SCREEN_Y));
	init_checks(env);
	env->render_mode = 0;
	env->sidebar = -1;
	env->selected = -1;
	env->state = 15252;
	load_textures(env);
	temp_env = env;
}
