/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_map.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/30 15:13:35 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/09 15:54:55 by ctrouve          ###   ########.fr       */
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

double	ft_deg_to_rad(double deg)
{
	return (deg * M_PI / 180.0);
}

t_3d	ft_rotate_vec3_x(t_3d v, double a)
{
	t_3d res;

	res.x = v.x;
	res.y = v.y * cos(a) + v.z * sin(a);
	res.z = -v.y * sin(a) + v.z * cos(a);
	return (res);
}

t_3d	ft_rotate_vec3_y(t_3d v, double a)
{
	t_3d res;

	res.x = v.x * cos(a) + v.z * sin(a);
	res.y = v.y;
	res.z = -v.x * sin(a) + v.z * cos(a);
	return (res);
}

t_3d	ft_rotate_vec3_z(t_3d v, double a)
{
	t_3d res;

	res.x = v.x * cos(a) - v.y * sin(a);
	res.y = v.x * sin(a) + v.y * cos(a);
	res.z = v.z;
	return (res);
}

/*
** rotates vec3 by rot (degrees) in XYZ order
*/

t_3d	ft_rotate_vec3(t_3d v, t_3d rot)
{
	v = ft_rotate_vec3_x(v, ft_deg_to_rad(rot.x));
	v = ft_rotate_vec3_y(v, ft_deg_to_rad(rot.y));
	v = ft_rotate_vec3_z(v, ft_deg_to_rad(rot.z));
	return (v);
}
