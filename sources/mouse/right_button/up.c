/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   up.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <dmalesev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:29:01 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/02 14:22:07 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	right_button_up(t_2i coords, void *param)
{
	t_env	*env;

	env = param;
	/*process_image(&env->sdl, &env->img[0], 1, env);
	process_image(&env->sdl, &env->img[1], 1, env);
	process_image(&env->sdl, &env->img[2], 1, env);*/
	(void)coords;
	env->mouse_state ^= 4;
	printf("RIGHT UP\n");
}
