/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadratic.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 13:18:34 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/19 13:19:07 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	assess_t(t_2d *t)
{
	double	temp;

	if (t->x < 0 && t->y < 0)
	{
		t->x = T_MAX;
		t->y = T_MAX;
		return (0);
	}
	if (t->x < 0)
		t->x = t->y;
	if (t->y < 0)
		t->y = T_MAX;
	if (t->x > t->y)
	{
		temp = t->x;
		t->x = t->y;
		t->y = temp;
	}
	return (1);
}

int	quadratic_equation(t_quadratic *quadratic, t_2d *t)
{
	double	discriminant;
	double	quad_a_c;

	quad_a_c = quadratic->a * quadratic->c;
	discriminant = ((quadratic->b * quadratic->b) - (4 * quad_a_c));
	if (discriminant < 0)
		return (0);
	else if (discriminant == 0)
	{
	}
	else
	{
		t->x = (-quadratic->b - sqrt(discriminant)) / (2 * quadratic->a);
		t->y = (-quadratic->b + sqrt(discriminant)) / (2 * quadratic->a);
	}
	return (assess_t(t));
}
