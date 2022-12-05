/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:07:49 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/05 11:48:38 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	load_textures(t_env *env)
{
	env->bmptxtr.wasd = SDL_LoadBMP("test.bmp");
	if (env->bmptxtr.wasd == NULL)
		close_prog(NULL, "Failed to load BMP texture...", -3);
	/* env->bmptxtr.slider = SDL_LoadBMP("slider.bmp");
	if (env->bmptxtr.slider == NULL)
		close_prog(NULL, "Failed to load BMP texture...", -3); */
}

void	init_main(t_env *env)
{
	char	*font_path;

	env->scene = ft_memalloc(sizeof(t_scene));
	if (env->scene == NULL)
		close_prog(NULL, "Malloc env.scene failed...", -1);
	font_path = "libraries/dm_bdf_render/examples/bdf_files/ic8x8u.bdf";
	font_path = "libraries/dm_bdf_render/examples/bdf_files/cascadia_code_semi_bold-12.bdf";
	env->font = load_font(font_path);
	if (env->font == NULL)
		close_prog(NULL, "Failed to load font...", -2);
	env->scene->resolution_range = (t_2i){0, 3};
	env->scene->resolution.x = env->scene->resolution_range.x;
	env->scene->resolution.y = env->scene->resolution_range.x;
	env->scene->accum_resolution.x = env->scene->resolution_range.x;
	env->scene->accum_resolution.y = env->scene->resolution_range.x;
	env->scene->accum_buffer = (t_3d *)malloc(sizeof(t_3d) * (SCREEN_X * SCREEN_Y));
	if (env->scene->accum_buffer == NULL)
		close_prog(NULL, "Failed to malloc for accum_buffer...", -4);
	env->scene->cam_hit_buffer = (t_cam_hit *)malloc(sizeof(t_cam_hit) * (SCREEN_X * SCREEN_Y));
	if (env->scene->cam_hit_buffer == NULL)
		close_prog(NULL, "Failed to malloc for cam_hit_buffer...", -5);
	env->render_mode = -1;
	env->sidebar = -1;
	env->selected = -1;
	env->state = 15252;
	load_textures(env);
}
