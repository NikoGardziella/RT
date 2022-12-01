/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:28:49 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/01 16:14:32 by ctrouve          ###   ########.fr       */
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

/*
** First we translate the coord into new_coord in a horizontal plan (0, 1, 0)
** then in 2D into this plan by negating the y coordinate.
*/
static t_2d	planar_map(t_object hit_object, t_3d hit_point)
{
	t_2d	coord_2d;
	t_3d	abc_param;
	double	d;
	double	k;
	t_3d	new_coord;

	abc_param.x = hit_object.axis.x;
	abc_param.y = hit_object.axis.y;
	abc_param.z = hit_object.axis.z;
	// abc_param.x = 0;
	// abc_param.y = 1;
	// abc_param.z = 0;
	d = hit_object.axis.x * hit_object.origin.x + hit_object.axis.y * hit_object.origin.y + hit_object.axis.z * hit_object.origin.z;
//	d = hit_object.origin.y;
	k = (d - (abc_param.x * hit_point.x) - (abc_param.y * hit_point.y) - (abc_param.z * hit_point.z)) /\
	 (abc_param.x * abc_param.x + abc_param.y * abc_param.y + abc_param.z * abc_param.z);
	new_coord.x = hit_point.x + k * abc_param.x;
	new_coord.y = hit_point.y + k * abc_param.y;
	new_coord.z = hit_point.z + k * abc_param.z;
	coord_2d.x = new_coord.x;
	coord_2d.y = new_coord.z;
	return (coord_2d);
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

//	height = hit_object->radius / 2;
	color_a.combined = hit_object->color.combined;
	color_b.combined = 0xFFFFFF;
	if (hit_object->type == SPHERE)
	{
		height = hit_object->radius / 2;
		uv_pos = spherical_map(*hit_object, hit_point);
		checker = init_checker(2 * height, height, color_a, color_b);
	}
	else
	{
		height = 10;
		uv_pos = planar_map(*hit_object, hit_point);
		checker = init_checker(height, height, color_a, color_b);
	}
	color_out = uv_pattern_at(checker, uv_pos.x, uv_pos.y);
	return (color_out);
}
