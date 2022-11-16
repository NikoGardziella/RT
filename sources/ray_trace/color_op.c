/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 17:08:55 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/15 15:31:52 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_rgba	ft_add_rgba(t_rgba c1, t_rgba c2)
{
	c1.r = (uint8_t)ft_min((c1.r + c2.r), 255);
	c1.g = (uint8_t)ft_min((c1.g + c2.g), 255);
	c1.b = (uint8_t)ft_min((c1.b + c2.b), 255);
	c1.a = (uint8_t)ft_max((c1.a + c2.a), 255);
	return (c1);
}

t_rgba	ft_make_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	t_rgba	color;

	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return (color);
}

t_rgba	ft_mul_rgba_rgba(t_rgba a, t_rgba b)
{
	t_rgba	color;

	color.r = (uint8_t)(b.r * a.r);
	color.g = (uint8_t)(b.g * a.g);
	color.b = (uint8_t)(b.b * a.b);
	color.a = (uint8_t)(b.a * a.a);
	return (color);
}

t_rgba	ft_mul_rgba(t_rgba c, double t)
{
	c.r = (uint8_t)(c.r * t);
	c.g = (uint8_t)(c.g * t);
	c.b = (uint8_t)(c.b * t);
	c.a = (uint8_t)(c.a * t);
	return (c);
}

static double	ft_lerp_d(double n1, double n2, double t)
{
	return (n1 + (n2 - n1) * t);
}

t_rgba	ft_lerp_rgba(t_rgba c1, t_rgba c2, double t)
{
	t_rgba	color;

	color.r = (uint8_t)ft_lerp_d(c1.r, c2.r, t);
	color.g = (uint8_t)ft_lerp_d(c1.g, c2.g, t);
	color.b = (uint8_t)ft_lerp_d(c1.b, c2.b, t);
	color.a = (uint8_t)ft_lerp_d(c1.a, c2.a, t);
	return (color);
}

/*
uint32_t	ft_get_color(t_rgba c)
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	r = 0;
	g = 0;
	b = 0;
	r = (uint8_t)(c.r * 255.0f) << 16;
	g = (uint8_t)(c.g * 255.0f) << 8;
	b = (uint8_t)(c.b * 255.0f);
	return (r | g | b);
}*/
