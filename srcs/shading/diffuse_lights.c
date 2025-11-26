/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse_lights.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 03:51:40 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

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

//get_light_attenuation:
//Determines how much light reaches the hit point from a given light
//Uses different shadowing models depending on flags.
//Returns false if light is fully blocked and should be skipped.
static bool	get_light_attenuation(t_hit_info *hit, t_light *light,
	t_color *atten)
{
	if ((g_renderer(NULL)->shading_flag & FLAG_TRANSPARENT_SHADOW) != 0u
		&& (g_renderer(NULL)->shading_flag & FLAG_SHADOW) == 0u)
	{
		*atten = (t_color){255, 255, 255};
		return (true);
	}
	if ((g_renderer(NULL)->shading_flag & FLAG_TRANSPARENT_SHADOW) != 0u
		&& (g_renderer(NULL)->shading_flag & FLAG_REFRACTION) != 0u)
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
	double	dr;
	double	dg;
	double	db;

	tmp = *light;
	dr = (double)light->color.r * (double)atten->r / 255.0;
	dg = (double)light->color.g * (double)atten->g / 255.0;
	db = (double)light->color.b * (double)atten->b / 255.0;
	tmp.color.r = (int)fmin(255.0, dr);
	tmp.color.g = (int)fmin(255.0, dg);
	tmp.color.b = (int)fmin(255.0, db);
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
	t_list			*node;
	t_light			*light;
	t_color			color;
	t_color			atten;

	color = compute_ambient_light(hit->hit_color, g_scene(NULL));
	node = g_scene(NULL)->lights;
	while (node)
	{
		light = (t_light *)node->content;
		if (!get_light_attenuation(hit, light, &atten))
		{
			node = node->next;
			continue ;
		}
		add_light_contribution(&color, hit, light, &atten);
		node = node->next;
	}
	color = color_scale_clamped(color, 1);
	return (color);
}
