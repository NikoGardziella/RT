/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_camera_to_file.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 21:43:17 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/17 21:52:18 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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

static void	write_object_type(int fd)
{
	char	*str;

	str = "OBJECT ";
	ft_putstr_fd(str, fd);
	str = "camera\n";
	ft_putstr_fd(str, fd);
}

void	write_camera_to_file(t_camera *camera, int fd)
{
	t_3d	look_at;

	write_object_type(fd);
	write_object_vector(fd, "\tpos ", &camera->ray.origin);
	look_at = add_vectors(camera->ray.forward, camera->ray.origin);
	write_object_vector(fd, "\tlook ", &look_at);
	write_object_float(fd, "\tfov ", (float)camera->fov);
}
