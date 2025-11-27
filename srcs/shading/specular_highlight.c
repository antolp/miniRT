/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   specular_highlight.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 19:54:53 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//for norm
static t_color	init_color(void)
{
	t_color	c;

	c.r = 0;
	c.g = 0;
	c.b = 0;
	return (c);
}

//is_in_shadow()
//if you can cast a ray from light to hit_point, and intersect any object
//the point is in the shadow of that object.
//scale that with lighting component and you get decent relatively cheap shadow
//(cheap means it doesnt need a lot of computing power)
//adding a very small sample of light dir avoids shadow acnee and doesn't seem
//to impact performance. 
//
//So, this function is an older version of the compute_shadow_attenuation()
//that's actually used to compute shadows now, it's only used to know if we 
//should draw specular highlights
//(since bitmask introduction, shadow tracing is also doable using this)
bool	is_in_shadow(t_vec3 point, t_vec3 light_pos)
{
	t_ray		shadow_ray;
	t_vec3		light_dir;
	double		light_dist;
	double		dist_to_hit;
	t_hit_info	hit;

	light_dir = vec_sub(light_pos, point);
	light_dist = vec_length(light_dir);
	light_dir = vec_normalize(light_dir);
	shadow_ray.origin = vec_add(point, vec_mul(light_dir, 1e-4));
	shadow_ray.direction = light_dir;
	if (get_closest_hit(&shadow_ray, &hit))
	{
		dist_to_hit = vec_length(vec_sub(hit.hit_point, shadow_ray.origin));
		if (dist_to_hit < light_dist)
			return (true);
	}
	return (false);
}

//should skip light if light can't reach point 
static bool	should_skip_light(t_vec3 p, t_light *light, unsigned int flags)
{
	if ((flags & FLAG_SHADOW) != 0)
	{
		if (is_in_shadow(p, light->position))
			return (true);
	}
	return (false);
}

//compute_specular_component():
//Computes the specular highlight from single light using the Blinn-Phong model
//
//First, gets the reflected incoming light vector
//Then uses the cosine of the angle between the view direction and 
//reflected light:
//   spec_angle = max(0, dot(view_dir, reflect_dir))
//returns high values when the view dir is close to the reflection angle.
//Then raises the cosine to a power based on the material's shininess:
//   spec = pow(spec_angle, shininess)
//higher shininess values result in sharper highlights.
//Finally scales the result by light intensity, material specular strength,
//and light color
//https://en.wikipedia.org/wiki/Specular_highlight
//https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model
static t_color	compute_specular_component(t_material *m, t_hit_info *hit,
	t_vec3 view_dir, t_light *light)
{
	t_color	c;
	t_vec3	light_dir;
	t_vec3	reflect_dir;
	double	spec_angle;
	double	spec;

	light_dir = vec_normalize(vec_sub(light->position, hit->hit_point));
	reflect_dir = vec_reflect(vec_mul(light_dir, -1.0), hit->normal);
	spec_angle = vec_dot(view_dir, reflect_dir);
	spec_angle = fmax(spec_angle, 0.0);
	spec = pow(spec_angle, m->shininess) * light->intensity;
	spec *= m->specular_strength;
	c.r = (int)(light->color.r * spec);
	c.g = (int)(light->color.g * spec);
	c.b = (int)(light->color.b * spec);
	return (c);
}

//for each light, check if the intersection point is in the shadow
//of another object
//if not, compute the specular highlight at that specific point on object
t_color	compute_specular_highlight(t_material *m,
			t_hit_info *hit, t_vec3 view_dir)
{
	t_color		result;
	t_list		*node;
	t_light		*light;
	t_color		tmp;

	result = init_color();
	node = g_scene(NULL)->lights;
	while (node)
	{
		light = (t_light *)node->content;
		if (should_skip_light(hit->hit_point, light,
				g_renderer(NULL)->shading_flag))
		{
			node = node->next;
			continue ;
		}
		tmp = compute_specular_component(m, hit, view_dir, light);
		result = color_add(result, tmp);
		node = node->next;
	}
	return (result);
}
