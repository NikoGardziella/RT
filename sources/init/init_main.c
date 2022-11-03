/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:07:49 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/03 15:53:20 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_main(t_env *env)
{
	char	*font_path;

	font_path = "libraries/dm_bdf_render/examples/bdf_files/ic8x8u.bdf";
	env->font = load_font(font_path);
	if (env->font == NULL)
		close_prog(NULL, "Failed to load font...", -2);
	env->scene->resolution = 5;
}
