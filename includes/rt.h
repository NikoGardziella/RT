/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/27 12:46:07 by dmalesev          #+#    #+#             */
/*   Updated: 2022/10/28 11:42:09 by dmalesev         ###   ########.fr       */
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

/*Typedef structs*/

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

#endif
