/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 14:50:38 by ngardzie          #+#    #+#             */
/*   Updated: 2022/11/16 16:42:27 by dmalesev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

// source: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel

t_3d get_refraction_ray(t_3d normal, t_3d ray_dir, double index)
{
	t_3d refraction_dir;
	double	NdotI;
	double	etai = 1;
	double	eta;
	double	k;

	NdotI = dot_product(ray_dir, normal);
	if(NdotI < -1)
		NdotI = -1;
	else if(NdotI > 1)
		NdotI = 1;
	if (index < 1)
		index = 1;
	if(NdotI < 0)
	{
	//	NdotI = NdotI * -1;
		eta = etai / index;
	}
	else
	{
	//	normal = scale_vector(normal,-1);
		eta = index / etai;
	}
	k = 1 - eta * eta * (1 - NdotI * NdotI);	
	if(k < 0)
		return(ray_dir);
	else
		refraction_dir = normalize_vector(add_vectors(scale_vector(ray_dir,eta) ,scale_vector(normal, (eta * NdotI - sqrt(k)))));
	return (refraction_dir);
}
