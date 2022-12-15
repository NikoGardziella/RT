/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_math.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/15 16:13:37 by pnoutere          #+#    #+#             */
/*   Updated: 2022/12/15 16:14:45 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_mat	multiply_matrices(t_mat *m1, t_mat *m2)
{
	t_mat	mat;
	int		i;
	int		j;

	j = 0;
	ft_bzero(&mat, sizeof(t_mat));
	while (j < 4)
	{
		i = 0;
		while (i < 4)
		{
			mat.m[j][i] = m1->m[j][0] * m2->m[0][i] + m1->m[j][1] * m2->m[1][i]
				+ m1->m[j][2] * m2->m[2][i] + m1->m[j][3] * m2->m[3][i];
			i++;
		}
		j++;
	}
	return (mat);
}

t_mat	add_matrices(t_mat *m1, t_mat *m2)
{
	t_mat	mat;
	int		i;
	int		j;

	j = 0;
	ft_bzero(&mat, sizeof(t_mat));
	while (j < 4)
	{
		i = 0;
		while (i < 4)
		{
			mat.m[j][i] = m1->m[j][i] + m2->m[j][i];
			i++;
		}
		j++;
	}
	return (mat);
}

t_mat	scale_matrix(t_mat *m1, double factor)
{
	t_mat	mat;
	int		i;
	int		j;

	j = 0;
	ft_bzero(&mat, sizeof(t_mat));
	while (j < 4)
	{
		i = 0;
		while (i < 4)
		{
			mat.m[j][i] = m1->m[j][i] * factor;
			i++;
		}
		j++;
	}
	return (mat);
}

void	matrix_multip(t_3d *in, t_3d *out, t_mat *matrix)
{
	double	temp;

	(*out).x = (*in).x * ((*matrix).m[0][0]) + (*in).y * ((*matrix).m[1][0])
		+ (*in).z * ((*matrix).m[2][0]) + ((*matrix).m[3][0]);
	(*out).y = (*in).x * ((*matrix).m[0][1]) + (*in).y * ((*matrix).m[1][1])
		+ (*in).z * ((*matrix).m[2][1]) + ((*matrix).m[3][1]);
	(*out).z = (*in).x * ((*matrix).m[0][2]) + (*in).y * ((*matrix).m[1][2])
		+ (*in).z * ((*matrix).m[2][2]) + ((*matrix).m[3][2]);
	temp = (*in).x * ((*matrix).m[0][3]) + (*in).y * ((*matrix).m[1][3])
		+ (*in).z * ((*matrix).m[2][3]) + ((*matrix).m[3][3]);
	if (temp != 0)
	{
		(*out).x /= temp;
		(*out).y /= temp;
		(*out).z /= temp;
	}
}
