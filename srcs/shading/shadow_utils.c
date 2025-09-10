/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#define MAX_SHADOW_STEPS 64

//caustic_gain():
//Approximates light concentration (caustic) through refractive objects shadow.
//the goal is to simulates how light becomes focused when exiting a refractive
//surface.
//
//The formula used is a simplified energy approximation:
//    gain ~= eta² * (cos_out / cos_in)
//- era is the index of refraction (ior)
//- cos_in is the angle between incoming ray and normal (on entry)
//- cos_out is the angle between outgoing ray and normal (on exit)
//
//This (kind of) models how rays compress or spread due to refraction:
//- eta² accounts for increased ray density in the less dense (slower ?) medium
//- (cos_out / cos_in) adjusts for angular distortion of the rays dir
//
//To avoid extreme values that often lead to weird artifacts or
//unrealistic brightness, the result is clamped between [0.5, 4.0],
//those are effectively magic numbers (0.2 on cos_out is one too lol).
//
//This is obviously not physically accurate, but it gives a decent
//approximation for caustic brightening in translucent shadows...
//
//reading this is very uselfull : 
//https://graphics.stanford.edu/courses/cs148-10-summer/docs/
//2006--degreve--reflection_refraction.pdf
//wikipedia pages on refraction and snell laws are too.
double	caustic_gain(double ior, double cos_in, double cos_out)
{
	double	gain;

	if (cos_in <= 0.0)
		cos_in = 0.0001;
	if (cos_out <= 0.0)
		cos_out = 0.0001;
	gain = (ior * ior) * (cos_out / cos_in);
	if (gain < 0.5)
		gain = 0.5;
	if (gain > 4.0)
		gain = 4.0;
	return (gain);
}

static void	search_candidates(t_hit_shadow_var *ctx, t_list *objects)
{
	t_object	*obj;
	double		t;

	while (objects)
	{
		obj = (t_object *)objects->content;
		if (obj != ctx->ignore)
		{
			t = ctx->t_min;
			if (obj->intersect(obj, ctx->ray, &t))
			{
				if (t > 1e-4 && t < ctx->t_min && t < ctx->max_t)
				{
					ctx->t_min = t;
					ctx->hit->object = obj;
				}
			}
		}
		objects = objects->next;
	}
}

bool	get_closest_hit_ignoring(t_ray *ray, double max_t,
	t_object *ignore, t_hit_info *hit)
{
	t_hit_shadow_var	ctx;
	t_list				*objects;

	ctx = (t_hit_shadow_var){ray, ignore, max_t, DBL_MAX, hit};
	hit->object = NULL;
	objects = g_scene(NULL)->objects;
	search_candidates(&ctx, objects);
	if (!hit->object)
		return (false);
	hit->hit_point = vec_add(ray->origin,
			vec_mul(ray->direction, ctx.t_min));
	hit->object->get_normal(hit->object,
		&hit->hit_point, &hit->normal);
	return (true);
}
