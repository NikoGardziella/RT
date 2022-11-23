/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   up.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <dmalesev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:27:30 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/17 13:03:07 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	left_button_up(void *param)
{
	t_env	*env;
	t_2i	mouse_coords;
	
	env = param;
	env->sel_element = 0;
	env->mouse.state ^= 1;
	if (env->sel_ray.object != NULL && env->selected == -1)
	{
		ft_bzero(&env->sel_ray, sizeof(t_ray));
		render_screen(env);
	}
	SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y);
}
