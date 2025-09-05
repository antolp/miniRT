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

//applies linear interpolation
//mix = (1−t)*a + t*b	where t belongs to [0,1]
t_color	color_lerp(t_color c1, t_color c2, double t)
{
	t_color	res;

	res.r = (int)fmin(255.0, c1.r * (1.0 - t) + c2.r * t);
	res.g = (int)fmin(255.0, c1.g * (1.0 - t) + c2.g * t);
	res.b = (int)fmin(255.0, c1.b * (1.0 - t) + c2.b * t);
	return (res);
}

//adds two colors into one, avoids overflow
t_color	color_add(t_color a, t_color b)
{
	t_color	res;

	res.r = (int)fmin(255.0, (double)a.r + (double)b.r);
	res.g = (int)fmin(255.0, (double)a.g + (double)b.g);
	res.b = (int)fmin(255.0, (double)a.b + (double)b.b);
	return (res);
}

//first, get surface color (futur implementation textured/bumpy render)
//then, diffuse from color
//then, reflection/refraction
//then, fresnel through schlick approx
//finally, spec highlight though blinn-phong
t_color	shade_pixel(t_ray *ray, t_hit_info *hit, int depth)
{
	(void)ray;
	(void)depth;
	t_color	final_color;
	t_vec3	view_dir;

	if (vec_dot(hit->normal, ray->direction) >= 0)
		hit->normal = vec_mul(hit->normal, -1);
	final_color = compute_diffuse_lighting(&hit->object->material, hit->hit_point, hit->normal);
	if ((g_renderer(NULL)->shading_flags & FLAG_SPECULAR) != 0u)
	{
		view_dir = vec_normalize(vec_mul(ray->direction, -1.0));
		final_color = color_add(final_color,
				compute_specular_lighting(&hit->object->material,
					hit->hit_point, hit->normal, view_dir));
	}
	return (final_color);
}
