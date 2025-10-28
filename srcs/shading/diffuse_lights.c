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

static t_color	color_mul_255(t_color a, t_color b)
{
	t_color	r;

	r.r = (int)fmin(255.0, (double)a.r * (double)b.r / 255.0);
	r.g = (int)fmin(255.0, (double)a.g * (double)b.g / 255.0);
	r.b = (int)fmin(255.0, (double)a.b * (double)b.b / 255.0);
	return (r);
}

t_color	compute_ambient_light(t_color c, t_scene *scene)
{
	t_color	ambient;

	ambient.r = c.r * scene->ambient_color.r / 255.0 * scene->ambient_ratio;
	ambient.g = c.g * scene->ambient_color.g / 255.0 * scene->ambient_ratio;
	ambient.b = c.b * scene->ambient_color.b / 255.0 * scene->ambient_ratio;
	return (ambient);
}

void	apply_diffuse(t_color *out, t_hit_info *hit, t_light *light)
{
	t_vec3	light_dir;
	t_color	c;
	double	diff;

	c = hit->hit_color;
	light_dir = vec_normalize(vec_sub(light->position, hit->hit_point));
	diff = fmax(0.0, vec_dot(hit->normal, light_dir)) * light->intensity;
	out->r += c.r * light->color.r / 255.0 * diff;
	out->g += c.g * light->color.g / 255.0 * diff;
	out->b += c.b * light->color.b / 255.0 * diff;
}

void	clamp_color(t_color *c)
{
	c->r = fmin(c->r, 255);
	c->g = fmin(c->g, 255);
	c->b = fmin(c->b, 255);
}

//get_light_attenuation:
//Determines how much light reaches the hit point from a given light
//Uses different shadowing models depending on flags.
//Returns false if light is fully blocked and should be skipped.
static bool	get_light_attenuation(t_hit_info *hit, t_light *light,
	t_color *atten)
{
	if ((g_renderer(NULL)->shading_flag & FLAG_TRANSPARENT_SHADOW) != 0u &&
		(g_renderer(NULL)->shading_flag & FLAG_SHADOW) == 0u)
	{
		*atten = (t_color){255, 255, 255};
		return (true);
	}
	if ((g_renderer(NULL)->shading_flag & FLAG_TRANSPARENT_SHADOW) != 0u &&
		(g_renderer(NULL)->shading_flag & FLAG_REFRACTION) != 0u)
	{
		*atten = compute_shadow_attenuation(hit->hit_point, light->position);
	}
	else if ((g_renderer(NULL)->shading_flag & FLAG_SHADOW) != 0u)
	{
		if (is_in_shadow(hit->hit_point, light->position))
			return (false);
		*atten = (t_color){255, 255, 255};
	}
	else
		*atten = (t_color){255, 255, 255};
	if (atten->r == 0 && atten->g == 0 && atten->b == 0)
		return (false);
	return (true);
}
// add_light_contribution:
// Applies diffuse lighting from a given light to the accumulated color.
// Light intensity is modulated by attenuation color.

static void	add_light_contribution(t_color *color, t_hit_info *hit,
	t_light *light, t_color *atten)
{
	t_light	tmp;

	tmp = *light;
	tmp.color = color_mul_255(light->color, *atten);
	apply_diffuse(color, hit, &tmp);
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
t_color	compute_diffuse_lighting(t_hit_info *hit)
{
	t_scene			*scene;
	t_list			*node;
	t_light			*light;
	t_color			color;
	t_color			atten;
	unsigned int	flags;

	scene = g_scene(NULL);
	flags = g_renderer(NULL)->shading_flag;
	color = compute_ambient_light(hit->hit_color, scene);
	node = scene->lights;
	while (node)
	{
		light = (t_light *)node->content;
		if (!get_light_attenuation(hit, light, &atten))
		{
			node = node->next;
			continue;
		}
		add_light_contribution(&color, hit, light, &atten);
		node = node->next;
	}
	clamp_color(&color);
	return (color);
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
// t_color	compute_diffuse_lighting(t_hit_info *hit)
// {
// 	t_scene		*scene;
// 	t_list		*node;
// 	t_light		*light;
// 	t_color		color;
// 	t_color		atten;
// 	t_light		tmp;
// 	unsigned int	flags;

// 	scene = g_scene(NULL);
// 	flags = g_renderer(NULL)->shading_flag;
// 	color = compute_ambient_light(hit->object->material, scene);
// 	node = scene->lights;
// 	while (node)
// 	{
// 		light = (t_light *)node->content;

// 		// --- Shadow logic selection ---
// 		if ((flags & FLAG_TRANSPARENT_SHADOW) != 0u)
// 			atten = compute_shadow_attenuation(hit->hit_point, light->position);
// 		else if ((flags & FLAG_SHADOW) != 0u)
// 		{
// 			if (is_in_shadow(hit->hit_point, light->position))
// 			{
// 				node = node->next;
// 				continue;
// 			}
// 			atten = (t_color){255, 255, 255};
// 		}
// 		else
// 			atten = (t_color){255, 255, 255};

// 		// --- Skip lights with full attenuation ---
// 		if (atten.r == 0 && atten.g == 0 && atten.b == 0)
// 		{
// 			node = node->next;
// 			continue;
// 		}

// 		// --- Apply diffuse lighting ---
// 		tmp = *light;
// 		tmp.color = color_mul_255(light->color, atten);
// 		apply_diffuse(&color,hit , &tmp);
// 		node = node->next;
// 	}
// 	clamp_color(&color);
// 	return (color);
// }

