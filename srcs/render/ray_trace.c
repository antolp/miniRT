/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_trace.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "rt.h"

//pour l'instant variables au lieu de call getset direct, pour lisibilité

//for each object in the scene, test the interesection
//hit point closer if the scalar is t smaller
//if interescted AND hit point closer to the camera than previous interesct,
//store new the resulted hit_point
//returns the closest object the ray has hit
bool	get_closest_hit(t_ray *ray, t_vec3 *hit_point, t_vec3 *normal, t_object **hit_obj)
{
	t_list		*objects;
	t_object	*obj;
	double		t;
	double		t_min;
	t_vec3		temp_hit;

	t_min = DBL_MAX;
	*hit_obj = NULL; 
	objects = g_scene(NULL)->objects;
	while (objects)
	{
		obj = (t_object *)objects->content;
		if (obj->intersect(obj, ray, &t) && t > 0.001 && t < t_min)
		{
			t_min = t;
			*hit_obj = obj;
		}
		objects = objects->next;
	}
	if (!*hit_obj)
		return (false);
	*hit_point = vec_add(ray->origin, vec_mul(ray->direction, t_min));
	(*hit_obj)->get_normal(*hit_obj, hit_point, normal);
	return (true);
}

//provisoire
t_color	shade_pixel(t_ray *ray, t_object *object, t_vec3 *hit, t_vec3 *normal, int depth)
{
	(void)ray;
	(void)hit;
	(void)normal;
	(void)depth;
	return(object->material->base_color);
}

t_color	trace_ray(t_ray *ray, int depth)
{
	t_object	*obj;
	t_vec3		hit_point;
	t_vec3		normal;

	if (!get_closest_hit(ray, &hit_point, &normal, &obj))
		return (g_scene(NULL)->background_color);
	return (shade_pixel(ray, obj, &hit_point, &normal, depth));
}

