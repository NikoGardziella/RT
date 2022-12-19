/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_object_info1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/19 12:56:16 by ngardzie          #+#    #+#             */
/*   Updated: 2022/12/19 12:56:29 by ngardzie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	density(char *line, t_object *object)
{
	char	*str;

	str = "density";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line = ft_strchr(line, ' ');
		if (line)
			object->density = (double)ft_atof(line);
		if (object->density < 1.0)
			object->density = 1.0;
		if (object->density > MAX_DENSITY)
			object->density = MAX_DENSITY;
		return (1);
	}
	return (0);
}

int	roughness(char *line, t_object *object)
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

int	lumen(char *line, t_object *object)
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
		return (1);
	}
	return (0);
}

int	radius(char *line, t_object *object)
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
