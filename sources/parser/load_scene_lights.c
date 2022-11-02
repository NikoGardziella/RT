/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_scene_lights.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/02 16:17:56 by ctrouve           #+#    #+#             */
/*   Updated: 2022/11/02 16:58:13 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	check_if_light(char *line)
{
	char	*str;

	str = "OBJECT";
	if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
	{
		line = ft_strstr(line, str);
		line += ft_strlen(str);
		str = "light";
		if (ft_strnequ(ft_strstr(line, str), str, ft_strlen(str)))
			return (0);
		return (-1);
	}
	return (-2);
}

static int	read_light(t_object *light, char *line)
{
	static int	reading;

	if (line == NULL)
		return (-1);
	if (reading == 0)
	{
		light->type = check_if_light(line);
		if (light->type == 0)
			reading = 1;
	}
	else if (reading == 1)
	{
		if (check_if_light(line) >= -1)
		{
			reading = 0;
			return (1);
		}
		read_object_info(line, light);
	}
	return (0);
}

t_list	*read_lights_file(int fd)
{
	t_object	light;
	t_list		*lights_list;
	int			ret;
	char		*line;

	ret = 1;
	lights_list = NULL;
	ft_bzero(&light, sizeof(t_object));
	light.type = -1;
	while (ret > 0)
	{	
		line = NULL;
		ret = get_next_line(fd, &line);
		if (ret == -1)
			break ;
		if (read_light(&light, line) == 1 || (ret == 0 && light.type == 0))
		{
			if (add_object(&objects_list, &object) == -1)
				ft_putendl("ERROR: Failed to add object to scene...");
			read_object(&object, line);
		}
		if (line != NULL)
			free(line);
	}
	return (lights_list);
}

/*similar to load_scene_objects but will constitute a linked list of only the lights*/
t_list	*load_scene_lights(char *path)
{
	t_list	*lights_list;
	int		fd;

	fd = open(path, O_RDONLY);
	if (fd < 0)
		return (NULL);

	lights_list = read_lights_file(fd);
	if (fd >= 0)
		close(fd);
	return (lights_list);
}
