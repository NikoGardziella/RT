/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   up.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:27:30 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/15 16:10:26 by pnoutere         ###   ########.fr       */
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
	SDL_GetMouseState(&mouse_coords.x, &mouse_coords.y);
}
