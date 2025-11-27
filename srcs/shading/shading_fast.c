/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading_fast.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 17:19:02 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/27 20:16:07 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	apply_diffuse_fast(t_color *out, t_hit_info *hit, t_light *light)
{
	t_vec3	L;
	double	diff;

	L = vec_normalize(vec_sub(light->position, hit->hit_point));
	diff = vec_dot(hit->normal, L);
	if (diff > 0.0)
	{
		diff = diff * light->intensity;
		out->r += hit->object->material.base_color.r
			* light->color.r / 255.0 * diff;
		out->g += hit->object->material.base_color.g
			* light->color.g / 255.0 * diff;
		out->b += hit->object->material.base_color.b
			* light->color.b / 255.0 * diff;
	}
}

t_color	shade_diffuse_fast(t_hit_info *hit)
{
	t_scene	*scene;
	t_list	*node;
	t_light	*light;
	t_color	out;

	scene = g_scene(NULL);
	out = compute_ambient_light(hit->object->material.base_color, scene);
	node = scene->lights;
	while (node)
	{
		light = (t_light *)node->content;
		apply_diffuse_fast(&out, hit, light);
		node = node->next;
	}
	out.r = fmin(out.r, 255.0);
	out.g = fmin(out.g, 255.0);
	out.b = fmin(out.b, 255.0);
	return (out);
}
