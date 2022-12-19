/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 12:24:32 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/19 12:21:43 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	scale_into_view(t_img *img, double *x, double *y)
{
	*x += 1;
	*y += 1;
	*x *= (double)img->dim.size.x / 2;
	*y *= (double)img->dim.size.y / 2;
}

t_proj	init_proj(double fov, t_2i *dim, t_2d *z_depth)
{
	t_proj	proj;

	proj.z_near = z_depth->x;
	proj.z_far = z_depth->y;
	proj.fov = fov;
	proj.asp_ratio = (double)dim->x / (double)dim->y;
	proj.fov_rad = (double)(1 / tan(fov / 2 / 180 * PI));
	return (proj);
}

t_3d	rotate_point(t_3d point, t_3d rot)
{
	t_3d	point_rot[3];
	t_mat	rmatrix[3];

	rmatrix[0] = init_rmatrix_x(rot.x);
	rmatrix[1] = init_rmatrix_y(rot.y);
	rmatrix[2] = init_rmatrix_z(rot.z);
	matrix_multip(&point, &point_rot[1], &rmatrix[0]);
	matrix_multip(&point_rot[1], &point_rot[2], &rmatrix[1]);
	matrix_multip(&point_rot[2], &point_rot[0], &rmatrix[2]);
	return (point_rot[0]);
}

t_3d	get_points(t_img *img, t_3d *xyz, t_3d *rot, t_proj *proj)
{
	t_3d	point_rot;
	t_3d	point_proj;
	t_mat	pmatrix;

	pmatrix = init_pmatrix(proj);
	point_rot = rotate_point(*xyz, *rot);
	point_rot.z -= 20.0f;
	matrix_multip(&point_rot, &point_proj, &pmatrix);
	scale_into_view(img, &point_proj.x, &point_proj.y);
	return (point_proj);
}
