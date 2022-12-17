/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_scene.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 18:26:41 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/17 21:53:15 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	save_scene(t_scene *scene, char *path)
{
	int		fd;

	if (remove(path) != 0)
		close_prog(NULL, "Failed to remove file...", -8);
	fd = open(path, O_CREAT | O_WRONLY, 0664);
	if (fd >= 0)
	{
		write_objects_to_file(scene->object_list, fd);
		write_camera_to_file(scene->camera, fd);
	}
	else
		close_prog(NULL, "Failed to open file for saving scene...", -6);
	ft_putstr("Succesfully saved scene to: ");
	ft_putendl(path);
	close(fd);
}
