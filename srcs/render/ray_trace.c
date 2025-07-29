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

typedef struct s_hit_info
{
	t_vec3			hit_point;
	t_vec3			normal;
	t_object		*object;
}	t_hit_info;

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
t_color	shade_pixel(t_ray *ray, t_hit_info *hit, int depth)
{
	(void)ray;
	(void)depth;
	return (compute_diffuse_lighting(hit->object->material, hit->hit_point, hit->normal));
}

t_color	trace_ray(t_ray *ray, int depth)
{
	t_hit_info	hit;

	if (!get_closest_hit(ray, &hit.hit_point, &hit.normal, &hit.object))
		return (g_scene(NULL)->background_color);
	// printf("%f %f %f\n",hit.hit_point.x, hit.hit_point.y, hit.hit_point.z);
	return (shade_pixel(ray, &hit, depth));
}
