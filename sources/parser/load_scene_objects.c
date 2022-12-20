/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_scene_objects.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 09:25:30 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/20 14:55:59 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	check_if_object(char *line)
{
	char	*str[7];

	str[0] = "OBJECT";
	if (ft_strnequ(ft_strstr(line, str[0]), str[0], ft_strlen(str[0])))
	{
		line = ft_strstr(line, str[0]);
		line += ft_strlen(str[0]);
		set_object_names(str);
		if (ft_strnequ(ft_strstr(line, str[0]), str[0], ft_strlen(str[0])))
			return (0);
		else if (ft_strnequ(ft_strstr(line, str[1]), str[1], ft_strlen(str[1])))
			return (1);
		else if (ft_strnequ(ft_strstr(line, str[2]), str[2], ft_strlen(str[2])))
			return (2);
		else if (ft_strnequ(ft_strstr(line, str[3]), str[3], ft_strlen(str[3])))
			return (3);
		else if (ft_strnequ(ft_strstr(line, str[4]), str[4], ft_strlen(str[4])))
			return (4);
		else if (ft_strnequ(ft_strstr(line, str[5]), str[5], ft_strlen(str[5])))
			return (5);
		else if (ft_strnequ(ft_strstr(line, str[6]), str[6], ft_strlen(str[6])))
			return (6);
		return (-1);
	}
	return (-2);
}

static t_object	setup_object(void)
{
	t_object	object;

	ft_bzero(&object, sizeof(t_object));
	object.roughness = 1.0f;
	object.density = MAX_DENSITY;
	object.type = -1;
	object.rgb_coords = (t_2i){-1, -1};
	return (object);
}

int	read_object(t_object *object, char *line)
{
	static int	reading;

	if (line == NULL)
		return (-1);
	if (reading == 0)
	{
		*object = setup_object();
		object->type = check_if_object(line);
		if (object->type >= 0)
			reading = 1;
	}
	else if (reading == 1)
	{
		if (check_if_object(line) >= -1)
		{
			reading = 0;
			return (1);
		}
		read_object_info(line, object);
	}
	return (0);
}

t_list	*read_scene_file(int fd)
{
	t_object	object;
	t_list		*objects_list;
	int			ret;
	char		*line;

	ret = 1;
	objects_list = NULL;
	while (ret > 0)
	{
		line = NULL;
		ret = get_next_line(fd, &line);
		if (ret == -1)
			break ;
		if (read_object(&object, line) == 1 || (ret == 0 && object.type >= 0))
		{
			if (add_object(&objects_list, &object) == -1)
				ft_putendl("ERROR: Failed to add object to scene...");
			read_object(&object, line);
		}
		if (line != NULL)
			free(line);
	}
	return (objects_list);
}

t_list	*load_scene_objects(char *path)
{
	t_list	*objects_list;
	int		fd;

	fd = open(path, O_RDWR);
	if (fd < 0)
		return (NULL);
	objects_list = read_scene_file(fd);
	close(fd);
	return (objects_list);
}
