/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmalesev <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 15:15:57 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/14 19:49:53 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_object	*unpack_light(t_list *lights, int index)
{
	int	current;

	current = 0;
	while (lights)
	{
		if (((t_object *)lights->content)->type == LIGHT)
		{
			if (current == index)
				return (((t_object *)lights->content));
			else
				current++;
		}
		lights = lights->next;
	}
	return (NULL);
}

t_3d	multiply_vectors(t_3d vector1, t_3d vector2)
{
	t_3d	multiplied;

	multiplied.x = vector1.x * vector2.x;
	multiplied.y = vector1.y * vector2.y;
	multiplied.z = vector1.z * vector2.z;
	return (multiplied);
}

double	clamp(double value, double min, double max)
{
	if (value < min)
		return (min);
	if (value > max)
		return (max);
	return (value);
}

t_3d	get_brdf_ray(t_3d normal, t_ray *ray, t_hit *hit)
{
	t_3d	vec;
	t_2d	indexes;

	vec = random_vector(normal, 1.0f);
	if(hit->object->roughness < 1.0)
	{
		/*float n1, n2, ndotr = dot(rd,n);
		
		if( ndotr > 0. ) {
			n1 = 1./1.5; n2 = 1.;
			n = -n;
		} else {
			n2 = 1./1.5; n1 = 1.;
		}
				
		float r0 = (n1-n2)/(n1+n2); r0 *= r0;
		float fresnel = r0 + (1.0 - r0) * pow(1.0 - abs(ndotr), 5.0);
		*/
		indexes = (t_2d){1.0, 1.0};
		if (hit->inside == 1)
			indexes.y = hit->object->density;
		else
			indexes.x = hit->object->density;
		vec = get_refraction_ray(normal, ray->forward, indexes);
		//if(hash1(&seed) < fresnel || m > 6.5)
		if (hit->object->density > MAX_DENSITY)
			vec = reflect_vector(ray->forward, normal);
	}
	return (vec);
}

void	trace_light_path(t_scene *scene)
{
	t_3d		color;
	t_3d		object_color;
	t_ray		ray;
	t_object	*light;
	t_hit		hit;
	int			i;

	light = unpack_light(scene->object_list, 0);
	ray.origin = light->origin;
	ray.forward = random_vector((t_3d){0.0, 1.0, 0.0}, 2.0f);
	ft_bzero(&scene->light_path, sizeof(t_light_path) * LIGHT_BOUNCES);
	color.x = (double)light->color.channel.r / 255.0 * (light->lumen * light->lumen);
	color.y = (double)light->color.channel.g / 255.0 * (light->lumen * light->lumen);
	color.z = (double)light->color.channel.b / 255.0 * (light->lumen * light->lumen);
	i = 0;
	while (i < LIGHT_BOUNCES)
	{
		if (intersects(&ray, scene->object_list, &hit, 0))
		{
			ray.origin = add_vectors(hit.point, scale_vector(hit.normal, BIAS));
			scene->light_path[i].origin = ray.origin;
			object_color.x = (double)hit.object->color.channel.r / 255.0;
			object_color.y = (double)hit.object->color.channel.g / 255.0;
			object_color.z = (double)hit.object->color.channel.b / 255.0;
			color = multiply_vectors(color, object_color);
			if(hit.object->roughness > 0.0)
				scene->light_path[i].color = color;
			scene->light_path[i].normal = hit.normal;
			ray.forward = get_brdf_ray(hit.normal, &ray, &hit);
		}
		else
			break;
		i += 1;
	}
}

t_3d	trace_eye_path(t_env *env, t_ray *ray, t_scene *scene, int camera_bounces)
{
	t_3d		calc_color;
	t_3d		max_color;
	t_3d		object_color;
	t_object	*light;
	t_hit		hit;
	t_ray		light_ray;
	t_3d		light_color;
	t_3d		temp1;

	calc_color = (t_3d){0.0, 0.0, 0.0};
	max_color = (t_3d){1.0, 1.0, 1.0};

	int		jdiff = 0;

	int	i;
	i = 0;
	while (i < camera_bounces)
	{
		light = unpack_light(scene->object_list, 0);
		if (intersects(ray, scene->object_list, &hit, 1) == 0)
		{
			calc_color.x *= 255;
			calc_color.y *= 255;
			calc_color.z *= 255;
			return (calc_color);
		}
		t_3d	normal;
		normal = hit.normal;
		if (hit.object->type == LIGHT)
		{
			object_color.x = hit.object->color.channel.r / 255 * (hit.object->lumen * hit.object->lumen);
			object_color.y = hit.object->color.channel.g / 255 * (hit.object->lumen * hit.object->lumen);
			object_color.z = hit.object->color.channel.b / 255 * (hit.object->lumen * hit.object->lumen);
			calc_color = add_vectors(calc_color, multiply_vectors(max_color, object_color));
			return (calc_color);
		}
		object_color.x = (double)hit.object->color.channel.r / 255.0;
		object_color.y = (double)hit.object->color.channel.g / 255.0;
		object_color.z = (double)hit.object->color.channel.b / 255.0;
		ray->origin = add_vectors(hit.point, scale_vector(normal, BIAS));
		ray->forward = get_brdf_ray(normal, ray, &hit);
		if(hit.object->roughness > 0.0 || dot_product(ray->forward, normal) < 0.0)
			max_color = multiply_vectors(max_color, object_color);
		light_ray.forward = random_vector((t_3d){1.0, 0.0, 0.0}, 2.0f);
		light_ray.origin = scale_vector(light_ray.forward, light->radius * random_rangef(0.0, 1.0, &env->state));
		light_ray.origin = add_vectors(light->origin, light_ray.origin);
		light_ray.forward = subtract_vectors(light_ray.origin, ray->origin);
		light_ray.origin = ray->origin;

		/*Direct light*/
		double	weight;
		double	distance;
		distance = vector_magnitude(light_ray.forward);
		light_ray.forward = normalize_vector(light_ray.forward);
		if(hit.object->roughness > 0.0 && distance < intersect_loop(&light_ray, scene->object_list, &hit, 0).x)
		{
			t_3d	light_to_ray;
			double	cos_alpha_max;

			light_to_ray = subtract_vectors(light->origin, ray->origin);
			cos_alpha_max = light->radius * light->radius;
			cos_alpha_max /= dot_product(light_to_ray, light_to_ray);
			cos_alpha_max = fmax(0.0, cos_alpha_max);
			cos_alpha_max = fmin(1.0, cos_alpha_max);
			cos_alpha_max = sqrt(1.0 - cos_alpha_max);
			weight = 2.0 * (1.0 - cos_alpha_max);
			light_color.x = (double)light->color.channel.r * (light->lumen * light->lumen) / 255.0;
			light_color.y = (double)light->color.channel.g * (light->lumen * light->lumen) / 255.0;
			light_color.z = (double)light->color.channel.b * (light->lumen * light->lumen) / 255.0;
			temp1 = multiply_vectors(max_color, light_color);
			weight *= fmin(1.0, fmax(0.0, dot_product(light_ray.forward, normal)));
			temp1 = scale_vector(temp1, weight);
			temp1 = divide_vector(temp1, jdiff - 1 + 2);
			calc_color = add_vectors(calc_color, temp1);
		}
		if(hit.object->roughness > 0.0)
		{
			int	i;

			i = 0;
			while (i < LIGHT_BOUNCES)// && scene->light_path[i].color.x != 0.0 && scene->light_path[i].color.y != 0.0 && scene->light_path[i].color.z != 0.0)
			{
				t_3d	lp = subtract_vectors(scene->light_path[i].origin, ray->origin);
				t_ray	lpn;
				lpn.forward = normalize_vector(lp);
				lpn.origin = ray->origin;
				light_color = scene->light_path[i].color;
				if(vector_magnitude(lp) < intersect_loop(&lpn, scene->object_list, &hit, 0).x)
				{
					weight = clamp(dot_product(lpn.forward, normal), 0.0, 1.0)
						* clamp(dot_product(scale_vector(lpn.forward, -1), scene->light_path[i].normal), 0.0, 1.0)
						* clamp(1.0 / dot_product(lp, lp), 0.0, 1.0);
					temp1 = multiply_vectors(max_color, light_color);
					temp1 = scale_vector(temp1, weight);
					temp1 = divide_vector(temp1, jdiff + i + 2);
					calc_color = add_vectors(calc_color, temp1);
				}
				i++;
			}
		}
		if (hit.object->roughness > 0.0)
			jdiff++;
		else
			jdiff = 0;
		i++;
	}
	calc_color.x *= 255;
	calc_color.y *= 255;
	calc_color.z *= 255;
	return (calc_color);
}
