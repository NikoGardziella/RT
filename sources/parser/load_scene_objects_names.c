/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_scene_objects_names.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 09:25:30 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/19 19:13:20 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	set_object_names(char **str)
{
	str[0] = "light";
	str[1] = "sphere";
	str[2] = "plane";
	str[3] = "cone";
	str[4] = "cylinder";
	str[5] = "box";
	str[6] = "disc";
}
