/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 19:54:56 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//https://en.wikipedia.org/wiki/Schlick%27s_approximation
//i don't understand this
//thank you mister Schlick
double	fresnel_schlick(double cos_theta, double ior)
{
	double	r0;

	r0 = pow((1.0 - ior) / (1.0 + ior), 2.0);
	return (r0 + (1.0 - r0) * pow(1.0 - cos_theta, 5.0));
}

//if fresnel toggled on :
//compute fresnel normally 
//if fresnel toggled off :
//flat reflectance, use F0 (Schlick at cosθ = 1, facing surface)
double	get_fresnel_weight(t_material *m, t_ray *ray, t_hit_info *hit)
{
	double	weight;
	double	cos_theta;
	double	f0;

	weight = m->reflectivity;
	if ((g_renderer(NULL)->shading_flag & FLAG_FRESNEL) != 0u)
	{
		cos_theta = fmax(0.0, -vec_dot(ray->direction, hit->normal));
		weight = weight * fresnel_schlick(cos_theta, m->ior);
	}
	else
	{
		f0 = fresnel_schlick(1.0, m->ior);
		weight = weight * f0;
	}
	return (fmax(0.0, fmin(1.0, weight)));
}

//used in refraction too
//mult by eps avoids weird artefacts, especially when no supersampling
t_ray	compute_reflected_ray(t_ray *ray, t_hit_info *hit)
{
	t_ray			reflect_ray;
	t_vec3			reflect_dir;

	reflect_dir = vec_normalize(vec_reflect(ray->direction, hit->normal));
	reflect_ray.origin = vec_add(hit->hit_point, vec_mul(reflect_dir, 0.001));
	reflect_ray.direction = reflect_dir;
	return (reflect_ray);
}

//took reflection from old codebase
//we trace a new ray from intersection point with a reflected direction
//hit something, return weighted from fresnel (will reflect more the higher the
//angle of incidence)
t_color	compute_reflection(t_material *m, t_ray *ray, t_hit_info *hit,
							t_color current)
{
	t_ray			reflect_ray;
	t_color			reflected;
	double			weight;

	if (ray->depth <= 0)
		return (current);
	if ((g_renderer(NULL)->shading_flag & FLAG_REFLECTION) == 0u)
		return (current);
	weight = get_fresnel_weight(m, ray, hit);
	reflect_ray = compute_reflected_ray(ray, hit);
	reflected = trace_ray(&reflect_ray, ray->depth - 1);
	return (color_lerp(current, reflected, weight));
}
