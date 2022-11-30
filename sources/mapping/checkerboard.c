/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:28:49 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/30 16:33:26 by ctrouve          ###   ########.fr       */
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

static t_2d	planar_map(t_3d p)
{
	t_2d	pos;
	t_3d	new_coord;

	

	pos.x = floor(p.x);
	pos.y = floor(p.z);
	return (pos);
}

/* If you want your checkers to look "square" on the sphere, be sure and set 
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
	height = hit_object->radius / 2;
	if (hit_object->type == SPHERE)
	{
		uv_pos = spherical_map(*hit_object, hit_point);
		checker = init_checker(2 * height, height, color_a, color_b);
	}
	else
	{
		uv_pos = planar_map(hit_point);
		checker = init_checker(height, height, color_a, color_b);
	}
	color_out = uv_pattern_at(checker, uv_pos.x, uv_pos.y);
	return (color_out);
}
