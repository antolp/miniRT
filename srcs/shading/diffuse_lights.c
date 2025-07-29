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

t_color	compute_diffuse_lighting(t_material *mat, t_vec3 point, t_vec3 normal)
{
	t_color		result;
	t_scene		*scene;
	t_list		*light_node;
	t_light		*light;
	t_vec3		light_dir;
	double		diff;

	scene = g_scene(NULL);
	result.r = mat->base_color.r * scene->ambient_color.r / 255.0 * scene->ambient_ratio;
	result.g = mat->base_color.g * scene->ambient_color.g / 255.0 * scene->ambient_ratio;
	result.b = mat->base_color.b * scene->ambient_color.b / 255.0 * scene->ambient_ratio;

	light_node = scene->lights;
	while (light_node)
	{
		light = light_node->content;
		light_dir = vec_sub(light->position, point);
		light_dir = vec_normalize(light_dir);

		// Lambertian diffuse: max(dot(N, L), 0)
		diff = fmax(0.0, vec_dot(normal, light_dir)) * light->intensity;;

		result.r += mat->base_color.r * light->color.r / 255.0 * diff;
		result.g += mat->base_color.g * light->color.g / 255.0 * diff;
		result.b += mat->base_color.b * light->color.b / 255.0 * diff;

		light_node = light_node->next;
	}

	result.r = fmin(result.r, 255);
	result.g = fmin(result.g, 255);
	result.b = fmin(result.b, 255);
	return (result);
}
