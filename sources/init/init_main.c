/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:07:49 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/19 21:22:43 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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
	env->scene->subframe_range = (t_2i){0, 3};
	env->scene->subframe.x = env->scene->subframe_range.x;
	env->scene->subframe.y = env->scene->subframe_range.x;
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
