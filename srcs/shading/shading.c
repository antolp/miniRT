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

//first, get surface color (futur implementation textured/bumpy render)
//then, diffuse from color
//then, reflection/refraction
//then, fresnel through schlick approx
//finally, spec highlight though blinn-phong
//
//	WORK IN PROGRESS, WILL NEED REWRITE
//	rewriting this as of now would be wasting time, since texturing hasn't been
//	introduced yet, and the function will need some adaptation to work with it.
t_color	shade_pixel(t_ray *ray, t_hit_info *hit, int depth)
{
	t_color	final_color;
	t_color	transmitted;
	t_color	diffuse_col;
	t_color	spec_col;
	t_vec3	view_dir;
	double	diffuse_scale;

	//if a ray is traced from inside a shape, it's behind its surface
	//the normal is calculation doesnt take that into account
	//shading rely heavily on normal vector, so we need to realign its direction
	//is_outside is used in refraction logic (entering/exiting the object's medium)
	hit->is_outside = 1;
	if (vec_dot(hit->normal, ray->direction) >= 0.0)
	{
		hit->is_outside = 0;
		hit->normal = vec_mul(hit->normal, -1.0);
	}

	//diffuse scale  will probably end up being removed
	//goal was to introduce less transparent material but it's pretty ugly honestly...
	// diffuse_scale = 1.0;
	// if ((g_renderer(NULL)->shading_flag & FLAG_REFRACTION) != 0u
	// 	&& hit->object->material.refractivity > 0.0)
	// 	diffuse_scale = 0.0;

	diffuse_col = compute_diffuse_lighting(hit);
	if (g_renderer(NULL)->shading_flag & FLAG_REFRACTION
	 	&& hit->object->material.refractivity > 0.0)
		diffuse_col = color_scale_clamped(diffuse_col, 0.0);

	//specular, local highlight on the surface
	spec_col = (t_color){0, 0, 0};
	if ((g_renderer(NULL)->shading_flag & FLAG_SPECULAR) != 0u)
	{
		view_dir = vec_normalize(vec_mul(ray->direction, -1.0));
		spec_col = compute_specular_highlight(&hit->object->material,
					hit, view_dir);
	}
	final_color = color_add(diffuse_col, spec_col);

	//either refraction or reflection
	//no ray splitting

	//first refraction
	//transmission to add refracted+tinted ON TOP, not overwrite specular
	if ((g_renderer(NULL)->shading_flag & FLAG_REFRACTION) != 0u
		&& hit->object->material.refractivity > 0.0)
	{
		t_color refracted;

		refracted = compute_refraction(&hit->object->material, ray, hit, depth);

		/* tint by base_color and mix by refractivity (your existing logic) */
		transmitted.r = (int)(refracted.r * (hit->object->material.base_color.r / 255.0));
		transmitted.g = (int)(refracted.g * (hit->object->material.base_color.g / 255.0));
		transmitted.b = (int)(refracted.b * (hit->object->material.base_color.b / 255.0));
		transmitted = color_lerp(refracted, transmitted, hit->object->material.refractivity);

		final_color = color_add(final_color, transmitted);
	}
	//else reflection, adding and lerp inside compute_reflection
	else if ((g_renderer(NULL)->shading_flag & FLAG_REFLECTION) != 0u)
	{
		ray->depth = depth;
		final_color = compute_reflection(&hit->object->material, ray, hit, final_color);
	}
	return (final_color);
}
