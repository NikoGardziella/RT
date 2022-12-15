/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canvas_from_ppm.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctrouve <ctrouve@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/12 12:44:40 by ctrouve           #+#    #+#             */
/*   Updated: 2022/12/15 11:54:20 by ctrouve          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	init_canvas(t_canvas *canvas)
{
	canvas->color_scale = 0;
	canvas->data = NULL;
	canvas->width = 0;
	canvas->height = 0;
	canvas->magic_nbr = 0;
}

int	read_magic_nbr(char *line)
{
	int		nbr;

	nbr = 0;
	if (line[0] == 'P')
	{
		nbr = -1;
		if (line[1] >= 1 && line[1] <= 6 && line[2] =='\n')
			nbr = line[1];
	}
	return (nbr);
}

t_2i	read_width_height(char *line)
{
	char	**str;
	t_2i	width_height;

	width_height.x = 0;
	width_height.y = 0;
	str = ft_strsplit(line, ' ');
	if (*str)
	{
		width_height.x = ft_atoi(str[0]);
		width_height.y = ft_atoi(str[1]);
	}
	else
	{
		printf("error in canvas width-height format, closing RT");
		close_prog(NULL, "wrong canvas width-height format...", -1);
	}
	return (width_height);
}

// to be continued !!!

int	add_pixel(t_list **pixel_list, t_color *pixel)
{
	if (*pixel_list == NULL)
	{
		*pixel_list = ft_lstnew((t_color *)pixel, sizeof(t_color));
		if (*pixel_list == NULL)
			return (-1);
	}
	else
	{
		if (ft_lstappnew(pixel_list, (t_color *)pixel, sizeof(t_color)) == -1)
			return (-1);
	}
	return (1);
}

int	add_number(t_list **int_list, int *number)
{
	if (*int_list == NULL)
	{
		*int_list = ft_lstnew((int *)number, sizeof(int));
		if (*int_list == NULL)
			return (-1);
	}
	else
	{
		if (ft_lstappnew(int_list, (int *)number, sizeof(int)) == -1)
			return (-1);
	}
	return (1);
}

void	build_int_list(t_list *int_list, char *line)
{
	char	**str;
	int		i;
	int		number;

	str = ft_strsplit(line, ' ');
	i = 0;
	number = 0;
	while (str[i])
	{
		number = ft_atoi(str[i]);
		if (add_number(&int_list, &number) == -1)
			close_prog(NULL, "failed adding numbers to the list in create_int_list", -1);
		i++;
	}
}

t_list	*create_pixel_list(t_list *int_list)
{
	t_color	pixel;
	t_list	*current;
	t_list	*pixel_list;

	current = int_list;
	pixel_list = NULL;
	while (current != NULL)
	{
		
		pixel.channel.r = (uint8_t)floor(ft_atoi(current->content));
		current = current->next;
		pixel.channel.g = (uint8_t)floor(ft_atoi(current->content));
		current = current->next;
		pixel.channel.b = (uint8_t)floor(ft_atoi(current->content));
		pixel.channel.a = 1.0;
		add_pixel(&pixel_list, &pixel);
		current = current->next;
	}
	return (pixel_list);
}

double	read_color_scale(char *line)
{
	double	color_scale;

	color_scale = 0.0;
	if (ft_atoi(line) >= 1 && ft_atoi(line) <= 255)
	{
		color_scale = ft_atoi(line) / 255.0;
	}
	return (color_scale);
}

int	ppm_parsing(t_canvas *canvas, int fd)
{
	int			ret;
	char		*line;
	int			magic_nbr;
	t_2i		width_height;
	double		color_scale;
	int			gnl_iter;
	t_list		*pixel_list;
	t_list		*int_list;

	magic_nbr = 0;
	width_height.x = 0;
	width_height.y = 0;
	color_scale = 0.0;
	init_canvas(canvas);
	ret = 1;
	gnl_iter = 0;
	pixel_list = NULL;
	int_list = NULL;
	while (ret > 0)
	{	
		line = NULL;
		ret = get_next_line(fd, &line);
		if (ret == -1)
			break ;
		if (gnl_iter == 0)
		{
			magic_nbr = read_magic_nbr(line);
			if (magic_nbr >= 0 && !canvas->magic_nbr)
				canvas->magic_nbr = magic_nbr;
			if (magic_nbr < 0 || (magic_nbr != 0 && magic_nbr != canvas->magic_nbr))
				break;
			gnl_iter++;
		}
		if (gnl_iter == 1)
		{
			width_height = read_width_height(line);
			if (width_height.x >> 0 && width_height.y >> 0 && !canvas->width && !canvas->height)
			{
				canvas->width = width_height.x;
				canvas->height = width_height.y;
			}
			if (!canvas->width || !canvas->height)
				break;
			gnl_iter++;
		}
		if (gnl_iter == 2)
		{
			color_scale = read_color_scale(line);
			if (color_scale >= 0)// && canvas->color_scale == NULL)
				canvas->color_scale = color_scale;
			if (color_scale < 0 || (color_scale != 0 && color_scale != canvas->color_scale))
				break;
			gnl_iter++;
		}
		if (gnl_iter >= 3)
		{
			build_int_list(int_list, line);
		}
		if (line != NULL)
			free(line); 
	}
	pixel_list = create_pixel_list(int_list);
	if (!pixel_list)
		return (0);
	canvas->data = pixel_list;
	return (1);
}

t_canvas	canvas_from_ppm(char *path)
{
	int			fd;
	t_canvas	*canvas;

	fd = open(path, O_RDONLY);
	if (fd <= 0)
		close_prog(NULL, "Open canvas path failed", -1);
	canvas = (t_canvas *)malloc(sizeof(t_canvas));
	if(canvas == NULL)
	{
		printf("no canvas, closing RT");
		close_prog(NULL, "canvas_from_ppm failed...", -1);
	}
	if (!ppm_parsing(canvas, fd))
	{
		printf("error in canvas file format, closing RT");
		close_prog(NULL, "wrong canvas file format...", -1);
	}
	close(fd);
	return (*canvas);
}