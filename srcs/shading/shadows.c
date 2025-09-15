/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#define MAX_SHADOW_STEPS 64

//apply_refraction_tint():
//Applies a tint to the refraction obviously
//Blends the current transmission color with the hit object’s base color,
//depending on its refractivity coefficient (k in [0, 1]).
//(It's scaled wrong, closer to 0 means perfect passthrough, it is unfixable)
//Also computes the light entry angle and applies a caustic gain multiplier
//to simulate brightening through refractive surfaces (see caustic_gain)
static void	apply_refraction_tint(t_color *trans, t_hit_info *hit,
	t_ray *ray, double *light_scale)
{
	double	k;
	t_vec3	n;
	double	cos_in;

	k = hit->object->material.refractivity;
	if (k < 0.0)
		k = 0.0;
	if (k > 1.0)
		k = 1.0;
	trans->r = (int)fmin(255.0, trans->r * (1.0 - k)
			+ hit->hit_color.r * k);
	trans->g = (int)fmin(255.0, trans->g * (1.0 - k)
			+ hit->hit_color.g * k);
	trans->b = (int)fmin(255.0, trans->b * (1.0 - k)
			+ hit->hit_color.b * k);
	n = hit->normal;
	if (vec_dot(n, ray->direction) > 0.0)
		n = vec_mul(n, -1.0);
	cos_in = -vec_dot(ray->direction, n);
	if (cos_in < 0.0)
		cos_in = 0.0;
	*light_scale *= caustic_gain(hit->object->material.ior, cos_in, 0.2);
}

//get_step_length():
//Computes the distance between the current ray origin and the hit point,
//ensures a minimum step size of SHADOW_EPS to avoid infinite loops
//(helper for norm, the rest of the logic hangs without this)
static double	get_step_length(t_vec3 *hit_point, t_vec3 *origin)
{
	double	len;

	len = vec_length(vec_sub(*hit_point, *origin));
	if (len < SHADOW_EPS)
		len = SHADOW_EPS;
	return (len);
}

//trace_shadow();
//Steps a ray through the scene toward the light, accumulating refraction tint
//and caustic brightening if refractive objects are encountered
//Stops on opaque objects or when the ray reaches the light
//Returns the final shadow attenuation color to apply
//(helper for norm)
static t_color	trace_shadow(t_ray *ray, t_color *trans,
	double *light_scale, double remain)
{
	t_hit_info	hit;
	double		step_len;
	int			steps;

	steps = 0;
	while (steps < MAX_SHADOW_STEPS)
	{
		if (!get_closest_hit_ignoring(ray, remain, NULL, &hit))
			break;
		step_len = get_step_length(&hit.hit_point, &ray->origin);
		if (hit.object->material.refractivity <= 0.0)
			return ((t_color){0, 0, 0});
		apply_refraction_tint(trans, &hit, ray, light_scale);
		ray->origin = vec_add(hit.hit_point, vec_mul(ray->direction,
			SHADOW_EPS));
		remain = remain - step_len;
		if (remain <= 0.0)
			break ;
		steps++;
	}
	return (color_scale_sat(*trans, *light_scale));
}

//init_shadow_ray():
//Initializes a shadow ray pointing from surface point to light
//Also computes the distance remaining to the light source (for stepping)
//Applies a small offset (SHADOW_EPS), avoids self-intersections
//(same trick was used on ray-object intersection, bit redundant eh)
static void	init_shadow_ray(t_ray *ray, t_vec3 point, t_vec3 light_pos,
	double *remain)
{
	ray->direction = vec_normalize(vec_sub(light_pos, point));
	ray->origin = vec_add(point, vec_mul(ray->direction, SHADOW_EPS));
	*remain = vec_length(vec_sub(light_pos, point));
}

//compute_shadow_attenuation:
//Entry point for transparent shadow logic
//currently doesn't allow for scaling using the object refractive component
//may or may not work on this again, it's a pain in the ass
//light scale used to serve a purpose, not sure honestly
t_color	compute_shadow_attenuation(t_vec3 point, t_vec3 light_pos)
{
	t_ray		ray;
	t_color		trans;
	double		remain;
	double		light_scale;

	trans = (t_color){255, 255, 255};
	light_scale = 1.0;
	init_shadow_ray(&ray, point, light_pos, &remain);
	return (trace_shadow(&ray, &trans, &light_scale, remain));
}
