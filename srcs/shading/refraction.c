/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/10/16 03:03:52 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

#define FRESNEL_SPLIT_THRESHOLD 0.05

/* compute_refracted_dir():
Computes the refracted direction of a ray hitting a surface,
using Snell's Law to model how light bends between two media.

The Snell's Law, a.k.a law of refraction, relates the angle of incidence 
and refraction:
	n1 * sin(θ1) = n2 * sin(θ2)
Where:
	- n1, n2 are the indices of refraction (ior)
	- θ1 is the angle between incident ray and normal
	- θ2 is the angle between refracted ray and normal

In vector form, we (not me) can derive the formula :
	T = eta * I + (eta * cos(θ1) - sqrt(k)) * N
	where:
		- T is the refracted direction
		- I is the incident direction (normalized)
		- N is the surface normal (normalized, pointing against I)
		- eta = n1 / n2
		- cos(θ1) = -dot(I, N)
		- k = 1 - eta² * (1 - cos²(θ1))
	If k < 0 -> Total Internal Reflection (no real solution, reflect inside)

ascii drawing :
        water (ior > 1.0)
          ⟍          .
            ⟍        |
    incident  ⟍      . 
      ray       ⟍ θ2 |
                  ⟍  .
                    ⟍|
 -------------------[x]----------------
                	    |\ 
                     . \ 
                     |θ1\ 
                     .   \ refracted 
                     |    \  ray
                     .     \ 
the angles aren't steep enough, it's just to give an idea
the formulas can be found in this form in the openGL docs :
	https://registry.khronos.org/OpenGL-Refpages/gl4/html/refract.xhtml */
int	compute_refracted_dir(t_vec3 in, t_vec3 n, double eta, t_vec3 *out_dir)
{
	double	cosi;
	double	k;
	t_vec3	dir;

	cosi = -vec_dot(in, n);
	if (cosi < 0.0)
		cosi = 0.0;
	k = 1.0 - eta * eta * (1.0 - cosi * cosi);
	if (k < 0.0)
		return (0);
	dir = vec_add(vec_mul(in, eta), vec_mul(n, (eta * cosi - sqrt(k))));
	*out_dir = vec_normalize(dir);
	return (1);
}

//is_refraction_disabled()
//If the ray recursion depth is exhausted or if the renderer has refracton
//explicitly disabled via flags, no refraction.
static bool	is_refraction_disabled(int depth, unsigned int flags)
{
	if (depth <= 0)
		return (true);
	if ((flags & FLAG_REFRACTION) == 0u)
		return (true);
	return (false);
}

//handle_total_internal_reflection()
//Handles the case where total internal reflection occurs and no refracted
//direction can be computed.
//When a ray hits a surface from a denser medium at a steep angle, it causes
//the ray to reflect entirely instead of refracting through, that's total
//internal reflection
//in practice, this gives very weird visuals, but it may be physically
//accurate
//
//              some medim (ior1
//           ⟍          .          ⟋
//             ⟍        |        ⟋
//     incident  ⟍      .      ⟋ reflected 
//       ray       ⟍ θ2 | θ1 ⟋      ray
//                   ⟍  .  ⟋
//                     ⟍|⟋
//  -------------------[x]----------------
//                 	    |
//       some other medium much denser (ior2 > ior1)
//
//https://en.wikipedia.org/wiki/Total_internal_reflection
static t_color	handle_total_internal_reflection(t_ray *ray,
	t_hit_info *hit, int depth)
{
	t_ray	reflect_ray;

	reflect_ray = compute_reflected_ray(ray, hit);
	return (trace_ray(&reflect_ray, depth - 1));
}

//trace_and_blend_refraction():
//Traces the refracted ray and optionally blends it with a reflected ray,
//depending on the Fresnel weight at the hit point.
//1. Computes and traces the refracted ray.
//If the Fresnel weight is low, return the scaled transmission only. This
//	makes the render more cost-effective, since on round objects most of
//	the drawn pixels (those facing the camera) will have almost no reflection.
//	The traced ray also has no depth, again decreasing cost.
//Otherwise, trace the reflected ray.
//Finally blend transmission and reflection according to Fresnel weight.
//
//OUTDATED
static t_color	trace_and_blend_refraction(t_material *m, t_ray *ray,
	t_hit_info *hit, t_vec3 refract_dir)
{
	t_ray	refract_ray;
	t_ray	reflect_ray;
	t_color	col_t;
	t_color	col_r;
	double	fw;

	fw = get_fresnel_weight(m, ray, hit);
	refract_ray.origin = vec_add(hit->hit_point,
			vec_mul(refract_dir, SHADOW_EPS));
	refract_ray.direction = refract_dir;
	col_t = trace_ray(&refract_ray, ray->depth - 1);
	// if (fw < FRESNEL_SPLIT_THRESHOLD)
	// 	return (color_scale_clamped(col_t, 1.0 - fw));
	reflect_ray = compute_reflected_ray(ray, hit);
	col_r = trace_ray(&reflect_ray, ray->depth - 1);
	col_t = color_scale_clamped(col_t, 1.0 - fw);
	col_r = color_scale_clamped(col_r, fw);
	return (color_add(col_t, col_r));
}

//compute_refraction():
//Entry point for computing refraction shading for a ray hit.
//First, skips if refraction is disabled or final depth.
//If not, computes the relative index of refraction (η, or eta).
//If total internal reflection occurs, fallback to reflection.
//Otherwise, trace transmission and handle Fresnel blending.
//https://raytracing.github.io/books/RayTracingInOneWeekend.html#dielectrics
//https://en.wikipedia.org/wiki/Refraction
t_color	compute_refraction(t_material *m, t_ray *ray, t_hit_info *hit,
	int depth)
{
	t_vec3		refract_dir;
	t_ray		refract_ray;
	double		eta;

	if (is_refraction_disabled(depth, g_renderer(NULL)->shading_flag))
		return ((t_color){0, 0, 0});
	if (hit->is_outside == 0)
		eta = m->ior / 1.0;
	else
		eta = 1.0 / m->ior;
	if (!compute_refracted_dir(ray->direction, hit->normal, eta, &refract_dir))
		return (handle_total_internal_reflection(ray, hit, depth));
	ray->depth = depth;
	return (trace_and_blend_refraction(m, ray, hit, refract_dir));
}
