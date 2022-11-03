/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/03 13:07:49 by dmalesev          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2022/11/03 15:37:46 by pnoutere         ###   ########.fr       */
=======
/*   Updated: 2022/11/03 15:20:21 by dmalesev         ###   ########.fr       */
>>>>>>> 187762f7c0e964da68bd515bdb7c858fee94ba0c
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_main(t_env *env)
{
<<<<<<< HEAD
	env->mouse_state = 0;
	env->keymap = 0;
=======
	char	*font_path;

	font_path = "libraries/dm_bdf_render/examples/bdf_files/ic8x8u.bdf";
	env->font = load_font(font_path);
	if (env->font == NULL)
		close_prog(NULL, "Failed to load font...", -2);
>>>>>>> 187762f7c0e964da68bd515bdb7c858fee94ba0c
	env->scene->resolution = 5;
}
