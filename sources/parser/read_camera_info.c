/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_camera_info.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 15:20:11 by dmalesev          #+#    #+#             */
/*   Updated: 2022/11/04 13:00:25 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	position(char *line, t_camera *camera)
{
	char	*str;

	str = "pos";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line = ft_strchr(line, ' ');
		if (line)
		{
			camera->ray.origin.x = (double)ft_atof(line++);
			line = ft_strchr(line, ' ');
		}
		if (line)
		{
			camera->ray.origin.y = (double)ft_atof(line++);
			line = ft_strchr(line, ' ');
		}
		if (line)
			camera->ray.origin.z = (double)ft_atof(line++);
		return (1);
	}
	return (0);
}

static int	look_at(char *line, t_camera *camera)
{
	t_3d	look_at;
	char	*str;

	str = "look";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line += ft_strlen(str);
		look_at.x = (double)ft_atof(line++);
		line = ft_strchr(line, ' ');
		look_at.y = (double)ft_atof(line++);
		line = ft_strchr(line, ' ');
		look_at.z = (double)ft_atof(line++);
		camera->ray.forward = subtract_vectors(look_at, camera->ray.origin);
		camera->ray.forward = normalize_vector(camera->ray.forward);
		return (1);
	}
	return (0);
}

static int	fov(char *line, t_camera *camera)
{
	char	*str;

	str = "fov";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line += ft_strlen(str);
		camera->fov = (double)ft_atof(line++);
		return (1);
	}
	return (0);
}

int	read_camera_info(char *line, t_camera *camera)
{
	if (position(line, camera))
		return (1);
	if (look_at(line, camera))
		return (1);
	if (fov(line, camera))
		return (1);
	return (0);
}
