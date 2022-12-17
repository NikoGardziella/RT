/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnoutere <pnoutere@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/11 15:15:57 by dmalesev          #+#    #+#             */
/*   Updated: 2022/12/17 21:02:24 by dmalesev         ###   ########.fr       */
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

double	fresnel_reflection(double theta_i, double n1, double n2)
{
	double	theta_t = asin(n1 / n2 * sin(theta_i));  // angle of refraction
	double	r_perp = ((n1 * cos(theta_i) - n2 * cos(theta_t)) / 
			(n1 * cos(theta_i) + n2 * cos(theta_t)));
	double r_par = ((n2 * cos(theta_i) - n1 * cos(theta_t)) / 
		(n2 * cos(theta_i) + n1 * cos(theta_t)));
	return (r_perp * r_perp + r_par * r_par) / 2;  // average of the perpendicular and parallel reflection coefficients
}

double	fresnel_reflection3(double theta, double n1, double n2)
{
	// Ensure that the incidence angle is within the range [0, 90] degrees
	double theta_deg = fmod(theta, M_PI / 2);
	if (theta > M_PI / 2) {
		theta_deg = M_PI - theta;
	}
	// Convert the angle to radians
	double theta_rad = theta_deg;

	// Calculate the Fresnel reflection coefficient
	double r_par = (n1 * cos(theta_rad) - n2 * sqrt(1 - pow(n1 / n2 * sin(theta_rad), 2))) / (n1 * cos(theta_rad) + n2 * sqrt(1 - pow(n1 / n2 * sin(theta_rad), 2)));
	double r_perp = (n1 * sqrt(1 - pow(n1 / n2 * sin(theta_rad), 2)) - n2 * cos(theta_rad)) / (n1 * sqrt(1 - pow(n1 / n2 * sin(theta_rad), 2)) + n2 * cos(theta_rad));
	double r = (r_par + r_perp) / 2;
	// Ensure that the reflection coefficient is between 0 and 1
	if (r < 0) {
		r = -r;
	}
	if (r > 1) {
		r = 1;
	}
	return r;
}

t_3d	get_brdf_ray(t_3d normal, t_ray *ray, t_hit *hit)
{
	t_3d	vec;
	t_2d	index;

	vec = random_vector(normal, 1.0f);
	if(hit->object->roughness < 1.0)
	{
		index = (t_2d){1.0, 1.0};
		if (hit->inside == 0)
			index.y = hit->object->density;
		else
			index.x = hit->object->density;
		double	angle;
		double	fresnel_probability;

		angle = angle_between_vectors(scale_vector(ray->forward, 1), normal);
		fresnel_probability = fresnel_reflection3(angle, index.x, index.y);
		if (hit->object->type == PLANE || hit->object->type == DISC)
			index = (t_2d){1.0, 1.0};
		//if (mid)
		//	printf("fresnel_probability %f\n", fresnel_probability);
		vec = get_refraction_ray(normal, ray->forward, index);
		ray->origin = add_vectors(hit->point, scale_vector(normal, BIAS * -1));
		if(random_rangef(0.0, 1.0, &temp_env->state) < fresnel_probability || hit->object->density == MAX_DENSITY)
		{
			vec = reflect_vector(ray->forward, normal);
			ray->origin = add_vectors(hit->point, scale_vector(normal, BIAS * 1));
		}
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
	ray.forward = random_vector((t_3d){0.0, -1.0, 0.0}, 2.0f);
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
	/*
	if (mid)
		printf("i************ NEW FRAME\n");
	if (mid == 1)
		printf("FIRST RAY x%f y%f z%f\n", ray->forward.x, ray->forward.y, ray->forward.z);
	*/
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
			object_color.x = (double)hit.object->color.channel.r / 255.0 * (hit.object->lumen * hit.object->lumen);
			object_color.y = (double)hit.object->color.channel.g / 255.0 * (hit.object->lumen * hit.object->lumen);
			object_color.z = (double)hit.object->color.channel.b / 255.0 * (hit.object->lumen * hit.object->lumen);
			calc_color.x *= 255;
			calc_color.y *= 255;
			calc_color.z *= 255;
			calc_color = add_vectors(calc_color, multiply_vectors(max_color, object_color));
			return (calc_color);
		}
		object_color.x = (double)hit.object->color.channel.r / 255.0;
		object_color.y = (double)hit.object->color.channel.g / 255.0;
		object_color.z = (double)hit.object->color.channel.b / 255.0;
		ray->origin = add_vectors(hit.point, scale_vector(normal, BIAS));
		ray->forward = get_brdf_ray(normal, ray, &hit);
		if(hit.object->roughness >= 0.0 || dot_product(ray->forward, normal) < 0.0)
			max_color = multiply_vectors(max_color, object_color);
		light_ray.forward = random_vector((t_3d){0.0, 1.0, 0.0}, 2.0f);
		light_ray.origin = scale_vector(light_ray.forward, light->radius * random_rangef(0.0, 1.0, &env->state));
		light_ray.origin = add_vectors(light->origin, light_ray.origin);
		light_ray.forward = subtract_vectors(light_ray.origin, ray->origin);
		light_ray.origin = ray->origin;

		/*Direct light*/
		double	weight;
		double	distance;
		distance = vector_magnitude(light_ray.forward);
		light_ray.forward = normalize_vector(light_ray.forward);
		(void)distance;
		(void)weight;
		(void)temp1;
		(void)light_color;
		/*
		if(hit.object->roughness > 0.0 && distance < intersect_loop(&light_ray, scene->object_list, &hit, 0).x)
		{
			t_3d	light_to_ray;
			double	cos_alpha_max;

			light_to_ray = subtract_vectors(light->origin, ray->origin);
			cos_alpha_max = 1;//light->radius * light->radius;
			cos_alpha_max /= dot_product(light_to_ray, light_to_ray);
			cos_alpha_max = fmax(0.0, cos_alpha_max);
			cos_alpha_max = fmin(1.0, cos_alpha_max);
			cos_alpha_max = sqrt(1.0 - cos_alpha_max);
			weight = 2.0 * (1.0 - cos_alpha_max);
			light_color.x = (double)light->color.channel.r / 255.0 * (light->lumen * light->lumen);
			light_color.y = (double)light->color.channel.g / 255.0 * (light->lumen * light->lumen);
			light_color.z = (double)light->color.channel.b / 255.0 * (light->lumen * light->lumen);
			temp1 = multiply_vectors(max_color, light_color);
			weight *= fmin(1.0, fmax(0.0, dot_product(light_ray.forward, normal)));
			temp1 = scale_vector(temp1, weight);
			temp1 = divide_vector(temp1, jdiff - 1 + 2);
			calc_color = add_vectors(calc_color, temp1);
		}
		*/
		/*if(hit.object->roughness > 0.0)
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
		}*/
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
