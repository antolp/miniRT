/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_trace.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/27 20:13:28 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//for each object in the scene, test the interesection
//hit point closer if the scalar is t smaller
//if interescted AND hit point closer to the camera than previous interesct,
//store new the resulted hit_point
//returns the closest object the ray has hit
bool	get_closest_hit(t_ray *ray, t_hit_info *hit)
{
	t_list		*objects;
	t_object	*obj;
	double		t;
	double		t_min;

	t_min = DBL_MAX;
	hit->object = NULL;
	objects = g_scene(NULL)->objects;
	while (objects)
	{
		obj = (t_object *)objects->content;
		t = t_min;
		if (obj->intersect(obj, ray, &t) && t > 1e-4 && t < t_min)
		{
			t_min = t;
			hit->object = obj;
		}
		objects = objects->next;
	}
	if (!hit->object)
		return (false);
	hit->hit_point = vec_add(ray->origin, vec_mul(ray->direction, t_min));
	(hit->object)->get_normal(hit->object, &hit->hit_point, &hit->normal);
	return (true);
}

t_color	trace_ray(t_ray *ray, int depth)
{
	t_hit_info	hit;

	if (!get_closest_hit(ray, &hit))
		return (get_background_color(ray->direction));
	return (shade_pixel(ray, &hit, depth));
}

//same as trace_ray(), but instead of following the full shading pipeline,
//the pixel has a slight shading as "half-lambertian" is performed
//
//V = normalize(-ray->direction)   vector from hit point to camera
//here, we use it like a light ray in the compute_diffuse logic
//Let a = dot(N, V)
//	if a =  1  surface fully faces the camera
//	if a =  0  nearly touches surface 
//	if a = -1  fully back-facing
//k = 0.5 + 0.5 * a, darker side isn't pitch black
t_color	trace_fast_ray(t_ray *ray, int none)
{
	t_hit_info	hit;

	(void)none;
	if (!get_closest_hit(ray, &hit))
		return (g_scene(NULL)->background_color);
	orient_normal(ray, &hit);
	return (shade_diffuse_fast(&hit));
}
