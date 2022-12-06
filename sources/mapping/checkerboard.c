/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:28:49 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/06 18:41:17 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "rt.h"

static t_checker	init_checker(double width, double height, t_color color_a, t_color color_b)
{
	t_checker	checker;

	checker.width = width;
	checker.height = height;
	checker.color_a = color_a;
	checker.color_b = color_b;
	return (checker);
}

static t_color	uv_pattern_at(t_checker checker, double u, double v)
{
	double	u2;
	double	v2;

	u2 = floor(u * checker.width);
	v2 = floor(v * checker.height);
	if((int)(u2 + v2) % 2 == 0)
		return (checker.color_a);
	else
		return (checker.color_b);
}

t_2d	spherical_map(t_object sphere, t_3d p)
{
	double	theta; // azimuthal angle -π < theta <= π
	double	phi; // polar angle 0 <= phi <= π
	double	raw_u; // -0.5 < raw_u <= 0.5
	t_2d	pos;

	theta = atan2(p.x - sphere.origin.x, p.z - sphere.origin.z);
	phi = acos((p.y - sphere.origin.y) / sphere.radius);
	raw_u = theta / (2 * PI);
	pos.x = 1 - (raw_u + 0.5); // 0 <= u < 1 we fix the direction of u. Subtract it from 1, so that it increases counterclockwise as viewed from above.
	pos.y = 1 - phi / PI;// we want v to be 0 at the south pole of the sphere, and 1 at the north pole so we have to "flip v over" by subtracting it from 1 to have 1 at north.
	return (pos);
}

static double	ft_deg_to_rad(double deg)
{
	return (deg * M_PI / 180.0);
}

t_3d	ft_rotate_vec3(t_3d v, t_3d rot)
{
	t_3d	res;

	rot.x = ft_deg_to_rad(rot.x);
	rot.y = ft_deg_to_rad(rot.y);
	rot.z = ft_deg_to_rad(rot.z);
	res.x = v.x;
	res.y = v.y * cos(rot.x) + v.z * sin(rot.x);
	res.z = -v.y * sin(rot.x) + v.z * cos(rot.x);
	v = (t_3d){res.x, res.y, res.z};
	res.x = v.x * cos(rot.y) + v.z * sin(rot.y);
	res.y = v.y;
	res.z = -v.x * sin(rot.y) + v.z * cos(rot.y);
	v = (t_3d){res.x, res.y, res.z};
	res.x = v.x * cos(rot.z) - v.y * sin(rot.z);
	res.y = v.x * sin(rot.z) + v.y * cos(rot.z);
	res.z = v.z;
	return (res);
}

t_2d	plane_checker(t_object object, t_3d coords)
{
	t_3d	new_coords;
	t_3d	rot;
	t_2d	uv_coords;

	new_coords.x = coords.x - object.origin.x;
	new_coords.y = coords.y - object.origin.y;
	new_coords.z = coords.z - object.origin.z;
	rot = (t_3d){-1 * object.axis.x, -1 * object.axis.y, -1 * object.axis.z};
	rot = normalize_vector(rot);
	//new_coords = ft_rotate_vec3(new_coords, rot);
	uv_coords.x = new_coords.x;
	uv_coords.y = new_coords.z;
	return (uv_coords);
}
/*
** If you want your checkers to look "square" on the sphere, be sure and set 
** the width to twice the height. This is because of how the spherical map is 
** implemented. While both u and v go from 0 to 1, v maps 1 to π, but u maps 
** 1 to 2π. -> width = 2*height in init_bw_checker
*/
t_color	define_checker_color(t_object *hit_object, t_3d hit_point)
{
	t_color		color_out;
	t_2d		uv_pos;
	t_checker	checker;
	t_color		color_a;
	t_color		color_b;
	double		height;

	color_a.combined = hit_object->color.combined;
	color_b.combined = 0xFFFFFF;
	if (hit_object->type == SPHERE)
	{
		height = hit_object->radius / 2;
		uv_pos = spherical_map(*hit_object, hit_point);
		checker = init_checker(2 * height, height, color_a, color_b);
		color_out = uv_pattern_at(checker, uv_pos.x, uv_pos.y);
	}
	else
	{
		height = 10;
		uv_pos = plane_checker(*hit_object, hit_point);
		checker = init_checker(height, height, color_a, color_b);
		color_out = uv_pattern_at(checker, uv_pos.x, uv_pos.y);
	}
	return (color_out);
}
