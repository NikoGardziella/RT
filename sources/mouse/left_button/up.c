/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   up.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <dmalesev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 13:27:30 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/11 17:52:17 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	left_button_up(void *param)
{
	t_env	*env;

	env = param;
	env->mouse.state ^= 1;
	env->sel_ray.object = NULL;
}
