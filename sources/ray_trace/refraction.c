/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngardzie <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 14:50:38 by ngardzie          #+#    #+#             */
/*   Updated: 2022/11/16 14:50:40 by ngardzie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

// source: https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/reflection-refraction-fresnel

t_3d refraction_ray1(t_hit *hit, t_ray *ray, double ior)
{
	t_ray *tmp1;

	tmp1 = ray;
	t_3d refraction_dir;
	t_3d Nrefr = hit->normal;
	t_3d I = hit->point;
	double NdotI = dot_product(I,Nrefr);

	if(NdotI < -1)
		NdotI = -1;
	else if(NdotI > 1)
		NdotI = 1;
	double etai = 1;
	double etat = ior;
	double tmp = 0.0;
	double eta;
	double k;
	if(NdotI < 0)
		NdotI = NdotI * -1;
	else
	{
		Nrefr = scale_vector(Nrefr,-1);
		tmp = etat;
		etat = etai;
		etai = tmp;
	}
	eta = etai / etat;
	k = 1 - eta * eta * (1 - NdotI * NdotI);	
	
	if(k < 0)
		return(ray->forward);
	else
		refraction_dir = normalize_vector(add_vectors(scale_vector(I,eta) ,scale_vector(Nrefr, (eta * NdotI - sqrt(k)))));
	return (refraction_dir);
}
