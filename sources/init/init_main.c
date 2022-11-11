/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:07:49 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/11 13:44:14 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	load_textures(t_env *env)
{
	env->bmptxtr.wasd = SDL_LoadBMP("test.bmp");
	if (env->bmptxtr.wasd == NULL)
		close_prog(NULL, "Failed to load BMP texture...", -3);
}

void	init_main(t_env *env)
{
	char	*font_path;

	font_path = "libraries/dm_bdf_render/examples/bdf_files/ic8x8u.bdf";
	font_path = "libraries/dm_bdf_render/examples/bdf_files/cascadia_code_semi_bold-12.bdf";
	env->font = load_font(font_path);
	if (env->font == NULL)
		close_prog(NULL, "Failed to load font...", -2);
	env->scene->resolution_range = (t_2i){0, 5};
	env->scene->resolution.x = env->scene->resolution_range.x;
	env->scene->resolution.y = env->scene->resolution_range.x;
	env->render_mode = -1;
	load_textures(env);
}
