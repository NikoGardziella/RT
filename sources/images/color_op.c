/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 17:08:55 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/03 09:49:52 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_rgba	ft_add_rgba(t_rgba c1, t_rgba c2)
{
	c1.r += c2.r;
	c1.g += c2.g;
	c1.b += c2.b;
	c1.a += c2.a;
	return (c1);
}

/*t_rgba	ft_make_rgba(double r, double g, double b, double a)
{
	t_rgba	color;

	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return (color);
}*/

t_rgba	ft_mul_rgba_rgba(t_rgba a, t_rgba b)
{
	a.r *= b.r;
	a.g *= b.g;
	a.b *= b.b;
	a.a *= b.a;
	return (a);
}

t_rgba	ft_mul_rgba(t_rgba c, double t)
{
	c.r *= t;
	c.g *= t;
	c.b *= t;
	c.a *= t;
	return (c);
}

int	ft_get_color(t_rgba c)
{
	int	r;
	int	g;
	int	b;

	r = 0;
	g = 0;
	b = 0;
	r = (int)(c.r * 255.0f) << 16;
	g = (int)(c.g * 255.0f) << 8;
	b = (int)(c.b * 255.0f);
	return (r | g | b);
}
