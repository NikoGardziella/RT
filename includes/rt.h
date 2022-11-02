/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 17:07:07 by pnoutere          #+#    #+#             */
/*   Updated: 2022/11/02 09:39:22 by dmalesev         ###   ########.fr       */
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
# define IMAGES 3

# ifndef PI
#  define PI 3.141592
# endif

/*Typedef enums*/

typedef enum e_obj_type
{
	LIGHT,
	SPHERE,
	PLANE,
	CONE,
	CYLINDER
}				t_obj_type;

/*Typedef structs*/

typedef struct s_proj
{
	double	z_near;
	double	z_far;
	double	fov;
	double	asp_ratio;
	double	fov_rad;
}				t_proj;

typedef struct s_mat
{
	double	m[4][4];
}				t_mat;

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

typedef union		u_color
{
	uint32_t		combined;
	uint8_t			channels[4];
}					t_color;

typedef struct s_object
{
	double		axis_length;
	double		radius;
	double		t;
	int			lumen;
	int			type;
	t_color		color;
	t_3d		axis;
	t_3d		end;
	t_3d		hit_point;
	t_3d		length;
	t_3d		normal;
	t_3d		origin;
	t_3d		position;
	t_3d		rotation;
}				t_object;

typedef struct s_hit
{
	t_3d		point;
	t_3d		normal;
	t_object	*object;
	t_3d		light_dir;
	double		distance;
	t_color		color;
}				t_hit;

typedef struct s_camera_info
{
	t_3d		v_up;
	t_3d		u;
	t_3d		v;
	t_3d		w;
	double		theta;
	double		half_height;
	double		half_width;
}				t_camera_info;

typedef struct s_camera
{
	t_3d		pos;
	t_3d		look_at;
	double		fov;
	double		scale;
	double		aspect;
	t_3d		horizontal;
	t_3d		vertical;
	t_3d		lower_left_corner;
}				t_camera;

typedef struct s_ray
{
	t_3d		origin;
	t_3d		direction;
	t_object	*origin_object;
}				t_ray;

typedef struct s_scene
{
	t_list		*objects_list;
	t_camera	*camera;
	t_uint		ambient_color;
}				t_scene;

typedef struct s_2d
{
	double	x;
	double	y;
}				t_2d;

typedef struct s_2f
{
	float	x;
	float	y;
}				t_2f;

typedef struct s_dim
{
	t_2i	size;
	t_2i	start;
	t_2i	end;
}				t_dim;

typedef struct s_img
{
	SDL_Surface		*surface;
	int				bits_per_pixel;
	int				line_length;
	t_dim			dim;
	void			*addr;
	void			(*draw_func)(struct s_img *img, void *param);
}				t_img;

typedef struct s_sdl
{
	SDL_Event		event;
	SDL_Window		*window;
	SDL_Surface		*screen;
}				t_sdl;

typedef struct s_env
{
	t_sdl	sdl;
	int		height;
	int		width;
	t_img	*img;
	t_scene	*scene;
}				t_env;

/*Parser Functions*/

t_list		*load_scene_objects(char *path);
int			add_object(t_list **objects, t_object *object);
int			read_object_info(char *line, t_object *object);
int			transformations(char *line, t_object *object);

/*Init functions*/

t_img		*create_images(size_t count);
t_mat		init_rmatrix_x(double angle_x);
t_mat		init_rmatrix_z(double angle_z);
t_mat		init_rmatrix_y(double angle_y);
t_mat		init_pmatrix(t_proj *proj);

/*Close and free functions*/

void		close_prog(void *param, char *exit_msg, int exit_code);
t_img		*free_images(t_img *img, size_t count);

/*Image functions*/

void		main_image(t_img *img, void *param);
void		sidebar_button(t_img *img, void *param);
void		ray_debugger(t_img *img, void *param);
void		render_scene(t_img *img, t_scene *scene);
t_uint		raycast(t_ray *ray, t_scene *scene, t_hit *hit);
t_ray		get_camera_ray(t_camera *camera, double x, double y);

/*Parser functions*/

t_3d		make_vector(double x, double y, double z);
t_camera	*load_scene_camera(char *path);
int			read_camera_info(char *line, t_camera *camera);
int			init_camera(t_camera *camera, t_3d pos, t_3d look_at, double fov);
t_list		*load_scene_objects(char *path);
int			read_object_info(char *line, t_object *object);
int			transformations(char *line, t_object *object);
void		process_image(t_sdl *sdl, t_img *img, int mode, void *param);
void		blit_surface(SDL_Surface *src, t_dim *srcrect, SDL_Surface *dest, t_dim *destrect);

/*Drawing functions*/

void		put_pixel(t_2i coords, t_uint color, void *param);
void		fill_image(t_img *img, t_uint color);

/*Intersect functions*/

void		quadratic(t_quadratic *q, int type);
double		intersect_plane(t_object plane, t_ray ray);
double		intersect_cone(t_object cone, t_ray ray);
double		intersect_sphere(t_object sphere, t_ray ray);
double		intersect_cylinder(t_object cyl, t_ray ray);
int			intersects(t_ray *ray, t_scene *scene, t_hit *hit);

/*Matrix transformations*/

t_3d		rotate_point(t_3d point, t_3d rot);
t_3d		get_points(t_img *img, t_3d *xyz, t_3d *rot, t_proj *proj);
t_proj		init_proj(double fov, t_2i *dim, t_2d *z_depth);
void		matrix_multip(t_3d *in, t_3d *out, t_mat *matrix);

#endif
