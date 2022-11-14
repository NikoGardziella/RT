/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   up.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <dmalesev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:27:30 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/14 14:04:33 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	left_button_up(void *param)
{
	t_env	*env;
	t_2i	mouse_coords;

	env = param;
	env->mouse.state ^= 1;
	env->sel_ray.object = NULL;
	ft_bzero(&env->sel_ray, sizeof(t_ray));
	SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y);
	if (coords_in_area(env->img[3].dim, mouse_coords) && env->sidebar == 1)
		printf("Placeholder\n");
	else if (coords_in_area(env->img[1].dim, mouse_coords))
		env->sidebar = 1;
	else if (coords_in_area(env->img[0].dim, mouse_coords))
	{
		render_screen(env);
	}
	else
		return ;
}
