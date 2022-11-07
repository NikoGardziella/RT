/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   down.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 12:39:02 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/07 09:41:59 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	coords_in_area(t_dim dim, t_2i coords)
{
	if (coords.x > dim.start.x && coords.x < dim.start.x + dim.size.x
			&& coords.y > dim.start.y && coords.y < dim.start.y + dim.size.y)
		return (1);
	return (0);
}

void	left_button_down(void *param)
{
	t_env	*env;
	t_2i	mouse_coords;

	env = param;
	env->mouse_state |= 1;
	SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y);
	if (coords_in_area(env->img[1].dim, mouse_coords))
		env->sidebar = 1;
	else
		env->sidebar = 0;
	put_images_to_screen(env);
}
