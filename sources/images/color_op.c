/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_op.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 17:08:55 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/04 14:16:03 by ctrouve          ###   ########.fr       */
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
	a.r = a.r * b.r / 255;
	a.g = a.g * b.g / 255;
	a.b = a.b * b.r / 255;
	a.r = a.r * b.r / 255;
	return (a);
}

t_rgba	ft_mul_rgba(t_rgba c, double t)
{
	c.r *= (uint8_t)(c.r * t);
	c.g *= (uint8_t)(c.g * t);
	c.b *= (uint8_t)(c.b * t);
	c.a *= (uint8_t)(c.a * t);
	return (c);
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
