/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_object_info.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/30 12:23:14 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/16 14:28:59 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	origin(char *line, t_object *object)
{
	char	*str;

	str = "origin";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line = ft_strchr(line, ' ');
		if (line)
		{
			object->origin.x = (double)ft_atof(line++);
			line = ft_strchr(line, ' ');
		}
		if (line)
		{
			object->origin.y = (double)ft_atof(line++);
			line = ft_strchr(line, ' ');
		}
		if (line)
			object->origin.z = (double)ft_atof(line++);
		return (1);
	}
	return (0);
}

static int	lumen(char *line, t_object *object)
{
	char	*str;

	str = "lumen";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line = ft_strchr(line, ' ');
		if (line)
			object->lumen = ft_atoi(line);
		if (object->lumen < 0)
			object->lumen = 0;
		if (object->type == LIGHT)
			object->radius = 0.5f;
		return (1);
	}
	return (0);
}

static int	radius(char *line, t_object *object)
{
	char	*str;

	str = "radius";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line = ft_strchr(line, ' ');
		if (line)
			object->radius = (double)ft_atof(line);
		if (object->radius < 0)
			object->radius = 0;
		return (1);
	}
	return (0);
}

static int	color(char *line, t_object *object)
{
	char	*str;

	str = "color";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line = ft_strchr(line, ' ');
		if (line)
			object->color.combined = (uint32_t)ft_atoh(line);
		return (1);
	}
	return (0);
}

static int	density(char *line, t_object *object)
{
	char	*str;

	str = "density";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line = ft_strchr(line, ' ');
		if (line)
			object->density = (double)ft_atof(line);
		if (object->density < 0)
			object->density = 0;
		if (object->density > 1)
			object->density = 1;
		return (1);
	}
	return (0);
}

static int	roughness(char *line, t_object *object)
{
	char	*str;

	str = "roughness";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line = ft_strchr(line, ' ');
		if (line)
			object->roughness = (double)ft_atof(line);
		if (object->roughness < 0)
			object->roughness = 0;
		if (object->roughness > 1)
			object->roughness = 1;
		return (1);
	}
	return (0);
}

int	read_object_info(char *line, t_object *object)
{
	if (origin(line, object))
		return (1);
	if (radius(line, object))
		return (1);
	if (lumen(line, object))
		return (1);
	if (color(line, object))
		return (1);
	if (transformations(line, object))
		return (1);
	if (density(line, object))
		return (1);
	if (roughness(line, object))
		return (1);
	return (0);
}
