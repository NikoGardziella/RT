/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_object_info.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/17 18:58:44 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/17 21:18:33 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

char	*get_object_type(t_object *object)
{
	if (object->type == LIGHT)
		return ("light\n");
	if (object->type == SPHERE)
		return ("sphere\n");
	if (object->type == PLANE)
		return ("plane\n");
	if (object->type == CYLINDER)
		return ("cylinder\n");
	if (object->type == DISC)
		return ("disc\n");
	if (object->type == CONE)
		return ("cone\n");
	if (object->type == BOX)
		return ("box\n");
	return (NULL);
}

char	*get_object_vector(const t_3d *vector, int axis)
{
	char	*str;

	str = NULL;
	if (axis == 0)
		str = ft_ftoa(vector->x, 5);
	else if (axis == 1)
		str = ft_ftoa(vector->y, 5);
	else if (axis == 2)
		str = ft_ftoa(vector->z, 5);
	if (str == NULL)
		close_prog(NULL, "Failed to malloc for str in get_object_info...", -7);
	return (str);
}

char	*get_object_float(float value)
{
	char	*str;

	str = ft_ftoa(value, 5);
	if (str == NULL)
		close_prog(NULL, "Failed to malloc for str in get_object_info...", -7);
	return (str);
}

char	*get_object_hex(unsigned int value)
{
	char	*str;

	str = ft_htoa(value);
	if (str == NULL)
		close_prog(NULL, "Failed to malloc for str in get_object_info...", -7);
	return (str);
}
