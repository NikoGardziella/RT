/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 15:13:35 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/30 15:48:38 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "rt.h"

t_color	define_normal_color(t_3d hit_normal)
{
	t_color	color_out;
	double	x;
	double	y;
	double	z;

	x = 127.5 + 127.5 * hit_normal.x;
	y = 127.5 + 127.5 * hit_normal.y;
	z = 127.5 + 127.5 * hit_normal.z;
	color_out.channel.b = (uint8_t)x;
	color_out.channel.g = (uint8_t)y;
	color_out.channel.r = (uint8_t)z;
	color_out.channel.a = 1;
	return (color_out);
}
