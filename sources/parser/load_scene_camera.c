/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_scene_camera.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:35:11 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/01 14:16:12 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_3d		make_vector(double x, double y, double z)
{

	t_3d	v;
	v.x = x;
	v.y = y;
	v.z = z;
	return (v);
}

double	ft_deg_to_rad(double deg)
{
	return (deg * M_PI / 180.0);
}

int	init_camera(t_camera *camera, t_3d pos, t_3d look_at, double fov)
{
	t_camera_info	ci;

	camera->aspect = (double)SCREEN_X / (double)SCREEN_Y;
	ci.v_up = make_vector(0, 1, 0.001);
	ci.w = normalize_vector(subtract_vectors(pos, look_at));
	ci.u = normalize_vector(cross_product(ci.v_up, ci.w));
	ci.v = cross_product(ci.w, ci.u);
	ci.theta = ft_deg_to_rad(fov);
	ci.half_height = tan(ci.theta / 2.0);
	ci.half_width = camera->aspect * ci.half_height;
	camera->lower_left_corner = subtract_vectors(pos, \
	subtract_vectors(scale_vector(ci.u, ci.half_width), \
	subtract_vectors(scale_vector(ci.v, ci.half_height), ci.w)));
	camera->horizontal = scale_vector(ci.u, ci.half_width * 2);
	camera->vertical = scale_vector(ci.v, ci.half_height * 2);
	return (1);
}

static int	check_if_camera(char *line)
{
	char	*str;

	str = "OBJECT";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line += ft_strlen(str);
		str = "camera";
		if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
			return (1);
		return (0);
	}
	return (-1);
}

static int	read_camera(t_camera *camera, char *line)
{
	static int	reading;

	if (line == NULL)
		return (-1);
	if (reading == 0)
	{
		if (check_if_camera(line) == 1)
			reading = 1;
	}
	else if (reading == 1)
	{
		if (check_if_camera(line) >= 0)
		{
			reading = 0;
			return (1);
		}
		read_camera_info(line, camera); /*function to extract pos, look_at and fov*/
	}
	return (0);
}

static t_camera	*read_camera_file(int fd, t_camera *camera)
{
	int			ret;
	char		*line;

	ret = 1;
	while (ret > 0)
	{	
		line = NULL;
		ret = get_next_line(fd, &line);
		if (ret == -1)
			break ;
		read_camera(camera, line);
		if (line != NULL)
			free(line);
	}
	/* at this point, camera->pos, look and fov are parsed*/
	init_camera(camera, camera->pos, camera->look_at, camera->fov);
	return (camera);
}

t_camera	*load_scene_camera(char *path)
{
	int			fd;
	t_camera	*camera;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		close_prog(NULL, "Open path failed", -1);
	camera = NULL;
	camera = read_camera_file(fd, camera);
	if (fd >= 0)
		close(fd);
	return (camera);
}
