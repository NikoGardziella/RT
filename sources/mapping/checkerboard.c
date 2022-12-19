/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/29 15:28:49 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/15 16:28:30 by ctrouve          ###   ########.fr       */
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

/*
t_2d	spherical_map(t_object sphere, t_3d p)
{
	double	theta; // azimuthal angle -π < theta <= π
	double	phi; // polar angle 0 <= phi <= π
	double	raw_u; // -0.5 < raw_u <= 0.5
	t_2d	uv_coords;

	theta = atan2(p.x - sphere.origin.x, p.z - sphere.origin.z);
	phi = acos((p.y - sphere.origin.y) / sphere.radius);
	raw_u = theta / (2 * PI);
	uv_coords.x = 1 - (raw_u + 0.5); // 0 <= u < 1 we fix the direction of u. Subtract it from 1, so that it increases counterclockwise as viewed from above.
	uv_coords.y = 1 - phi / PI;// we want v to be 0 at the south pole of the sphere, and 1 at the north pole so we have to "flip v over" by subtracting it from 1 to have 1 at north.
	return (uv_coords);
}
*/

t_2d		spherical_map(t_object sphere, t_3d p)
{
	double		theta;
	double		phi;
	t_2d		mapped;

	theta = atan2(-(p.z - sphere.origin.z), p.x - sphere.origin.x);
	phi = acos(-(p.y - sphere.origin.y) / sphere.radius);
	mapped.x = ((theta + M_PI) / (2 * M_PI));
	mapped.y = (1 - phi / M_PI);
	// mapped.x = (mapped.x * sphere->material.scale) + sphere->material.offset.x;
	// mapped.y = (mapped.y * sphere->material.scale) + sphere->material.offset.y;
	return (mapped);
}


t_2d	plane_map(t_object plane, t_3d coords)
{
	t_2d	uv_coords;
	t_3d	plane_normal;
	t_3d	u_axis;
	t_3d	v_axis;

	plane_normal = (t_3d){plane.axis.x, plane.axis.y, plane.axis.z};
	plane_normal = normalize_vector(plane_normal);
	if (plane_normal.x != 0 && plane_normal.y != 0)
		u_axis = normalize_vector((t_3d){plane_normal.y, -1 * plane_normal.x, 0});
	else if (plane_normal.x != 0 && plane_normal.y == 0)
		u_axis = normalize_vector((t_3d){plane_normal.z, -1 * plane_normal.x, 0});
	else if (plane_normal.x == 0 && plane_normal.y != 0)
		u_axis = normalize_vector((t_3d){plane_normal.y, -1 * plane_normal.z, 0});
	else
		u_axis = normalize_vector((t_3d){plane_normal.z, -1 * plane_normal.z, 0});
	v_axis = cross_product(plane_normal, u_axis);
	uv_coords.x = dot_product(u_axis, coords);
	uv_coords.y = dot_product(v_axis, coords);
	return (uv_coords);
}

t_2d	cylinder_map(t_object cylinder, t_3d coords)
{
	t_3d		new_x_axis;
	t_3d		new_y_axis;
	t_3d		new_z_axis;
	t_3d		new_coords;
	double		theta; // azimuthal angle -π < theta <= π
	double		raw_u;
	t_2d		uv_coords;
	t_3d		ori_to_hit;
	t_3d		rot;

	ori_to_hit = subtract_vectors(coords, cylinder.origin);
	rot = normalize_vector(cylinder.axis);
	new_y_axis = rot;
	new_coords.y = dot_product(new_y_axis, ori_to_hit);
	new_x_axis = ft_rotate_vec3((t_3d){1.0, 0.0, 0.0}, rot);
	new_x_axis = normalize_vector(new_x_axis);
	new_z_axis = cross_product(new_x_axis, new_y_axis);
	new_z_axis = normalize_vector(new_z_axis);
	new_coords.x = dot_product(new_x_axis, ori_to_hit);
	new_coords.z = dot_product(new_z_axis, ori_to_hit);

	theta = atan2(new_coords.x, new_coords.z);
	raw_u = theta / (2 * M_PI);
	uv_coords.x = (1 - (raw_u + 0.5));
	uv_coords.y = floor(new_coords.y);
	return (uv_coords);
}

/*
** If you want your checkers to look "square" on the sphere, be sure and set 
** the width to twice the height. This is because of how the spherical map is 
** implemented. While both u and v go from 0 to 1, v maps 1 to π, but u maps 
** 1 to 2π. -> width = 2*height in init_bw_checker
*/
t_color	define_checker_color(t_hit *hit)
{
	t_color		color_out;
	t_2d		uv_pos;
	t_checker	checker;
	t_color		color_a;
	t_color		color_b;
	double		width;
	double		height;

	width = 1;
	color_a.combined = hit->object->color.combined;
	color_b.combined = 0xFFFFFF;
	if (hit->object->type == SPHERE)
	{
		height = hit->object->radius;
		uv_pos = spherical_map(*hit->object, hit->point);
		checker = init_checker(2 * height, height, color_a, color_b);
		color_out = uv_pattern_at(checker, uv_pos.x, uv_pos.y);
	}
	else if (hit->object->type == PLANE)
	{
		height = 0.1;
		uv_pos = plane_map(*hit->object, hit->point);
		checker = init_checker(height, height, color_a, color_b);
		color_out = uv_pattern_at(checker, uv_pos.x, uv_pos.y);
	}
	else
	{
		// width = 2 * M_PI * hit->object->radius;
		// height = hit->object->radius;
		width = 2 * M_PI * hit->object->radius;
		height = 1;
		uv_pos = cylinder_map(*hit->object, hit->point);
		checker = init_checker(width, height, color_a, color_b);
		color_out = uv_pattern_at(checker, uv_pos.x, uv_pos.y);
	}
	return (color_out);
}
