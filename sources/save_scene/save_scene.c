/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 18:26:41 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/17 21:26:28 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	write_object_hex(int fd, char *value_name, unsigned int value)
{
	char	*str;

	ft_putstr_fd(value_name, fd);
	str = get_object_hex(value);
	ft_putstr_fd(str, fd);
	free(str);
	ft_putchar_fd('\n', fd);
}

static void	write_object_float(int fd, char *value_name, float value)
{
	char	*str;

	ft_putstr_fd(value_name, fd);
	str = get_object_float(value);
	ft_putstr_fd(str, fd);
	free(str);
	ft_putchar_fd('\n', fd);
}

static void	write_object_vector(int fd, char *vector_name, t_3d *vector)
{
	char	*str;

	ft_putstr_fd(vector_name, fd);
	str = get_object_vector(vector, 0);
	ft_putstr_fd(str, fd);
	free(str);
	ft_putchar_fd(' ', fd);
	str = get_object_vector(vector, 1);
	ft_putstr_fd(str, fd);
	free(str);
	ft_putchar_fd(' ', fd);
	str = get_object_vector(vector, 2);
	ft_putstr_fd(str, fd);
	free(str);
	ft_putchar_fd('\n', fd);
}

static void	write_to_scene_file(t_scene *scene, int fd)
{
	t_list		*object_list;
	t_object	*object;
	char		*str;
	int			type;

	object_list = scene->object_list;
	while (object_list != NULL)
	{
		object = (t_object *)object_list->content;
		type = object->type;
		str = "OBJECT ";
		ft_putstr_fd(str, fd);
		str = get_object_type(object);
		ft_putstr_fd(str, fd);
		write_object_vector(fd, "\torigin ", &object->origin);
		write_object_vector(fd, "\taxis ", &object->axis);
		write_object_float(fd, "\tradius ", (float)object->radius);
		if (type == LIGHT)
			write_object_float(fd, "\tlumen ", (float)object->lumen);
		else
		{
			write_object_float(fd, "\troughness ", (float)object->roughness);
			write_object_float(fd, "\tdensity ", (float)object->density);
		}
		write_object_hex(fd, "\tcolor ", object->color.combined);
		object_list = object_list->next;
	}
}

void	save_scene(t_scene *scene, char *path)
{
	int		fd;

	fd = open(path, O_CREAT | O_WRONLY, 0664);
	if (fd >= 0)
		write_to_scene_file(scene, fd);
	else
		close_prog(NULL, "Failed to open file for saving scene...", -6);
	ft_putstr("Succesfully saved scene to: ");
	ft_putendl(path);
	close(fd);
}
