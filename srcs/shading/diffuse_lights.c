/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse_lights.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//is_in_shadow()
//if you can cast a ray from light to hit_point, and intersect any object
//the point is in the shadow of that object.
//scale that with lighting component and you have decent (relatively) cheap shadows.
//(cheap means it doesnt need a lot of computing power)
bool	is_in_shadow(t_vec3 point, t_vec3 light_pos)
{
	t_ray		shadow_ray;
	t_vec3		light_dir;
	double		light_dist;
	t_hit_info	hit;

	light_dir = vec_sub(light_pos, point);
	light_dist = vec_length(light_dir);
	light_dir = vec_normalize(light_dir);

	shadow_ray.origin = vec_add(point, light_dir); //vec_mul(light_dir, 0.001));
	shadow_ray.direction = light_dir;

	if (get_closest_hit(&shadow_ray, &hit))
	{
		double dist_to_hit = vec_length(vec_sub(hit.hit_point, shadow_ray.origin));
		if (dist_to_hit < light_dist)
			return (true);
	}
	return (false);
}

t_color	compute_ambient_light(t_material *mat, t_scene *scene)
{
	t_color	ambient;

	ambient.r = mat->base_color.r * scene->ambient_color.r / 255.0 * scene->ambient_ratio;
	ambient.g = mat->base_color.g * scene->ambient_color.g / 255.0 * scene->ambient_ratio;
	ambient.b = mat->base_color.b * scene->ambient_color.b / 255.0 * scene->ambient_ratio;
	return (ambient);
}

void	apply_diffuse(t_color *out, t_material *mat, t_light *light,
			t_vec3 point, t_vec3 normal)
{
	t_vec3	light_dir;
	double	diff;

	light_dir = vec_normalize(vec_sub(light->position, point));
	diff = fmax(0.0, vec_dot(normal, light_dir)) * light->intensity;

	out->r += mat->base_color.r * light->color.r / 255.0 * diff;
	out->g += mat->base_color.g * light->color.g / 255.0 * diff;
	out->b += mat->base_color.b * light->color.b / 255.0 * diff;
}

void	clamp_color(t_color *c)
{
	c->r = fmin(c->r, 255);
	c->g = fmin(c->g, 255);
	c->b = fmin(c->b, 255);
}

//compute_diffuse_lighting()
//computes ambient + diffuse lighting for a surface point
//first, ambiant light
//then for each light :
//	- If the point is not in shadow w.r.t. that light,
//	adds Lambertian (diffuse) lighting based on light angle.
//
//https://en.wikipedia.org/wiki/Lambertian_reflectance
//(cheap but cool)
t_color	compute_diffuse_lighting(t_material *mat, t_vec3 point, t_vec3 normal)
{
	t_scene		*scene;
	t_list		*node;
	t_light		*light;
	t_color		color;

	scene = g_scene(NULL);
	color = compute_ambient_light(mat, scene);

	node = scene->lights;
	while (node)
	{
		light = (t_light *)node->content;
		if (!is_in_shadow(point, light->position))
			apply_diffuse(&color, mat, light, point, normal);
		node = node->next;
	}
	clamp_color(&color);
	return (color);
}
