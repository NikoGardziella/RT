/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:46:07 by dmalesev          #+#    #+#             */
/*   Updated: 2022/10/28 16:50:27 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include "libft.h"
# include "SDL.h"
# include "dm_2d.h"
# include "dm_vectors.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <time.h>

# define SCREEN_X 1000
# define SCREEN_Y 600
# define T_MAX 100000000.0f
# define IMAGES 2

# ifndef PI
#  define PI 3.141592
# endif


/*Typedef a.k.a custom types*/

typedef struct s_quadratic
{
	t_3d	w;
	t_3d	h;
	t_3d	subtr_top_bot;
	double	m;
	double	discr;
	double	t0;
	double	t1;
	double	a;
	double	b;
	double	c;
	double	q;
}	t_quadratic;

typedef enum e_obj_type
{
	PLANE,
	SPHERE,
	CYLINDER,
	CONE,
	LIGHT,
	CAMERA
}				t_obj_type;

typedef union		u_color
{
	uint32_t		combined;
	uint8_t			channels[4];
} 					t_color;

typedef struct s_object
{
	t_obj_type	type;
	t_3d		position;
	t_3d		rotation;
	t_3d		length;
	t_color		color;
	double		radius;
	t_3d		origin;
	t_3d		end;
	t_3d		normal;
	t_3d		hit_point;
}				t_object;

typedef struct s_ray
{
	t_3d		origin;
	t_3d		direction;
}				t_ray;

typedef struct s_dim
{
	t_2i	size;
	t_2i	start;
	t_2i	end;
}				t_dim;

typedef struct s_img
{
	SDL_Texture		*txtr;
	int				bits_per_pixel;
	int				line_length;
	t_dim			dim;
	void			*addr;
	void			(*draw_func)(struct s_img *img, void *param);
}				t_img;

typedef struct s_sdl
{
	SDL_Event		event;
	SDL_Renderer	*renderer;
	SDL_Window		*window;
}	t_sdl;

typedef struct s_env
{
	t_sdl	sdl;
	t_img	*img;
}	t_env;

/*Init functions*/

t_img	*create_images(SDL_Renderer *renderer, size_t count);

/*Close and free functions*/

void	close_prog(void *param, char *exit_msg, int exit_code);
t_img	*free_images(t_img *img, size_t count);

/*Image functions*/

void	main_image(t_img *img, void *param);
void	sidebar_button(t_img *img, void *param);

/*Drawing functions*/

void	put_pixel(t_2i coords, t_uint color, void *param);
void	fill_image(t_img *img, t_uint color);

/*Intersect functions*/
void	sphere_calculation(t_object sphere, t_ray ray);
void	cone_calculation(t_object cone, t_ray ray);
void	quadratic(t_quadratic *q, int type);
double	intersect_plane(t_object plane, t_ray ray);

#endif
