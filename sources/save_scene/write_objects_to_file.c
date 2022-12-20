/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_objects_to_file.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 21:37:28 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/17 21:53:45 by dmalesev         ###   ########.fr       */
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

static void	write_object_type(int fd, t_object *object)
{
	char	*str;

	str = "OBJECT ";
	ft_putstr_fd(str, fd);
	str = get_object_type(object);
	ft_putstr_fd(str, fd);
}

void	write_objects_to_file(t_list *object_list, int fd)
{
	t_object	*object;
	int			type;
	t_3d		axis;


	while (object_list != NULL)
	{
		object = (t_object *)object_list->content;
		type = object->type;
		write_object_type(fd, object);
		write_object_vector(fd, "\torigin ", &object->origin);
		axis = scale_vector(object->axis, object->axis_length);
		write_object_vector(fd, "\taxis ", &axis);
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
