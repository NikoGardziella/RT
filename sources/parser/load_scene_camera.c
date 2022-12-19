/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_scene_camera.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/31 14:35:11 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/17 21:29:03 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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
		read_camera_info(line, camera);
	}
	return (0);
}

void	set_camera(t_camera *cam)
{
	cam->ray.origin = (t_3d){0.0f, 0.0f, 0.0f};
	cam->ray.forward = (t_3d){0.0f, 0.0f, -1.0f};
	cam->fov = 60;
}

static t_camera	*read_camera_file(int fd, t_camera *camera)
{
	int		ret;
	char	*line;

	ret = 1;
	set_camera(camera);
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
	return (camera);
}

t_camera	*load_scene_camera(char *path)
{
	int			fd;
	t_camera	*camera;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		close_prog(NULL, "Open path failed", -1);
	camera = malloc(sizeof(t_camera));
	camera = read_camera_file(fd, camera);
	if (camera == NULL)
		close_prog(NULL, "Read_camera_file failed...", -1);
	if (fd >= 0)
		close(fd);
	return (camera);
}
