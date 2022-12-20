/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   selected_element.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 09:48:27 by pnoutere          #+#    #+#             */
/*   Updated: 2022/12/20 09:57:46 by pnoutere         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	prog_clock(t_env *env)
{
	t_2i		coords;
	t_2i		rgb_coords;
	uint32_t	rgb;

	prog_lock_checks(env, &rgb_coords, &rgb);
	check_if_element_six(env, rgb_coords, rgb, &coords);
	if (env->sel_element == 5)
	{
		env->sel_ray.object->lumen = 1 + (((double)MAX_LUMEN - 1)
				/ env->img[7].dim.size.x * (env->mouse.pos.x
					- env->img[7].dim.start.x));
		env->sel_ray.object->lumen = fmax(env->sel_ray.object->lumen, 0);
		env->sel_ray.object->lumen = fmin(
				env->sel_ray.object->lumen, MAX_LUMEN);
		render_screen(env);
	}
}

static t_2i	get_rgb_coords(t_env *env, t_2i current_coords)
{
	t_2i	coords;

	coords = current_coords;
	coords.x -= env->img[8].dim.start.x;
	coords.y -= env->img[8].dim.start.y;
	coords.x = ft_max(coords.x, 0);
	coords.y = ft_max(coords.y, 0);
	coords.x = ft_min(coords.x, env->img[8].dim.size.x - 1);
	coords.y = ft_min(coords.y, env->img[8].dim.size.y - 1);
	return (coords);
}

void	check_if_four(t_env *env, t_2i *rgb_coords, uint32_t *rgb)
{
	if (env->sel_element == 4)
	{
		*rgb_coords = get_rgb_coords(env, env->mouse.pos);
		env->sel_ray.object->rgb_coords = *rgb_coords;
		*rgb = (rgb_slider(&env->img[8], rgb_coords));
		env->sel_ray.object->color.combined = shade_picker(&env->img[9],
				&env->sel_ray.object->shade_coords, *rgb);
		render_screen(env);
	}
}

void	prog_lock_checks(t_env *env, t_2i *rgb_coords, uint32_t *rgb)
{
	if (env->sel_element == 2)
	{
		env->sel_ray.object->roughness = 1.0f / (double)env->img[6].dim.size.x
			* (float)(env->mouse.pos.x - env->img[6].dim.start.x);
		env->sel_ray.object->roughness = fmax(
				env->sel_ray.object->roughness, 0.0f);
		env->sel_ray.object->roughness
			= fmin(env->sel_ray.object->roughness, 1.0f);
		render_screen(env);
	}
	if (env->sel_element == 3)
	{
		env->sel_ray.object->density = 1 + (((double)MAX_DENSITY - 1)
				/ env->img[7].dim.size.x
				* (float)(env->mouse.pos.x - env->img[7].dim.start.x));
		env->sel_ray.object->density = fmax(env->sel_ray.object->density, 1.0f);
		env->sel_ray.object->density = fmin(
				env->sel_ray.object->density, MAX_DENSITY);
		render_screen(env);
	}
	check_if_four(env, rgb_coords, rgb);
}

void	check_if_element_six(t_env *env, t_2i rgb_coords,
	uint32_t rgb, t_2i *coords)
{
	if (env->sel_element == 6)
	{
		*coords = env->mouse.pos;
		coords->x -= env->img[9].dim.start.x;
		coords->y -= env->img[9].dim.start.y;
		coords->x = ft_max(coords->x, 0);
		coords->y = ft_max(coords->y, 0);
		coords->x = ft_min(coords->x, env->img[9].dim.size.x - 1);
		coords->y = ft_min(coords->y, env->img[9].dim.size.y - 1);
		env->sel_ray.object->shade_coords = *coords;
		rgb_coords = get_rgb_coords(env, env->sel_ray.object->rgb_coords);
		rgb = rgb_slider(&env->img[8], &rgb_coords);
		env->sel_ray.object->color.combined = shade_picker(
				&env->img[9], coords, rgb);
		render_screen(env);
	}
}
