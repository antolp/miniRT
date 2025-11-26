/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 03:56:01 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_color	shade_diffuse(t_hit_info *hit)
{
	t_color			c;
	unsigned int	flags;

	c = hit->hit_color;
	flags = g_renderer(NULL)->shading_flag;
	if ((flags & FLAG_DIFFUSE) != 0u)
		c = compute_diffuse_lighting(hit);
	if ((flags & FLAG_REFRACTION) != 0u
		&& hit->object->material.refractivity > 0.0)
		c = color_scale_clamped(c, 0.0);
	return (c);
}

static t_color	shade_specular_hl(t_ray *ray, t_hit_info *hit)
{
	t_color		spec_hl;
	t_vec3		view_dir;
	uint32_t	flags;

	spec_hl = (t_color){0, 0, 0};
	flags = g_renderer(NULL)->shading_flag;
	if ((flags & FLAG_SPECULAR) != 0u)
	{
		view_dir = vec_normalize(vec_mul(ray->direction, -1.0));
		spec_hl = compute_specular_highlight(&hit->object->material,
				hit, view_dir);
	}
	return (spec_hl);
}

//refracted contribution: trace, tint by hit_color, lerp by refractivity
static t_color	shade_refraction(t_ray *ray, t_hit_info *hit, int depth)
{
	t_color	refr;
	t_color	tinted;
	double	k;

	if ((g_renderer(NULL)->shading_flag & FLAG_REFRACTION) == 0u)
		return ((t_color){0, 0, 0});
	if (hit->object->material.refractivity <= 0.0)
		return ((t_color){0, 0, 0});
	refr = compute_refraction(&hit->object->material, ray, hit, depth);
	tinted.r = (int)(refr.r * (hit->hit_color.r / 255.0));
	tinted.g = (int)(refr.g * (hit->hit_color.g / 255.0));
	tinted.b = (int)(refr.b * (hit->hit_color.b / 255.0));
	k = hit->object->material.refractivity;
	return (color_lerp(refr, tinted, k));
}

//reflection contribution, compute_reflection blends into base color
//to allow "metalic" or "glossy", none-perfect mirrors (but cheap)
static t_color	shade_reflection(t_ray *ray, t_hit_info *hit,
	t_color base, int depth)
{
	if ((g_renderer(NULL)->shading_flag & FLAG_REFLECTION) == 0u)
		return (base);
	ray->depth = depth;
	return (compute_reflection(&hit->object->material, ray, hit, base));
}

//shade_pixel():
//Main entry to the shading pipeline, shades each pixel according to
//material properties of hit point, scene configurations normal
//steps :
//	-orient normal of camera facing back of surface
//	-get surface color from texture if any
//	-apply bump map if any
//	-get diffuse from color (Lambert + shadows/transparent shadows)
//	-get spec highlight (Blinn-Phong)
//	-add those two into a new pixel color
//	-compute either refraction or reflection (or none) :
//		-refraction + fresnel if refractive
//		-reflection + fresnel if non-refractive and reflective
t_color	shade_pixel(t_ray *ray, t_hit_info *hit, int depth)
{
	t_color	final_color;
	t_color	add_ref;
	t_color	diffuse_col;
	t_color	spec_col;

	orient_normal(ray, hit);
	hit->hit_color = surface_color(hit);
	apply_bump(hit);
	diffuse_col = shade_diffuse(hit);
	spec_col = shade_specular_hl(ray, hit);
	final_color = color_add(diffuse_col, spec_col);
	add_ref = shade_refraction(ray, hit, depth);
	if (add_ref.r != 0 || add_ref.g != 0 || add_ref.b != 0)
		final_color = color_add(final_color, add_ref);
	else
		final_color = shade_reflection(ray, hit, final_color, depth);
	return (final_color);
}
