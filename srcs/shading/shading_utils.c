/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 03:53:00 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//orient_normal();
//if normal same direction as ray vector, shading wil leak
//from below surface (e.g. inside of circle, behind a plane/triangle, etc)
//fix it by flipping normal, also tell the refraction logic that the ray
//is "inside" the object (which is stupid, you are never "inside" a surface)
void	orient_normal(t_ray *ray, t_hit_info *hit)
{
	double	dp;

	hit->is_outside = 1;
	dp = vec_dot(hit->normal, ray->direction);
	if (dp >= 0.0)
	{
		hit->is_outside = 0;
		hit->normal = vec_mul(hit->normal, -1.0);
	}
}

t_color	color_scale_sat(t_color c, double s)
{
	t_color	r;
	double	x;

	if (s < 0.0)
		s = 0.0;
	x = c.r * s;
	if (x > 255.0)
		x = 255.0;
	r.r = (int)x;
	x = c.g * s;
	if (x > 255.0)
		x = 255.0;
	r.g = (int)x;
	x = c.b * s;
	if (x > 255.0)
		x = 255.0;
	r.b = (int)x;
	return (r);
}

//not to norm
//needs rewrite
t_color	color_scale_clamped(t_color c, double s)
{
	t_color	r;
	double	x;

	if (s < 0.0)
		s = 0.0;
	if (s > 1.0)
		s = 1.0;
	x = c.r * s;
	if (x > 255.0)
		x = 255.0;
	r.r = (int)x;
	x = c.g * s;
	if (x > 255.0)
		x = 255.0;
	r.g = (int)x;
	x = c.b * s;
	if (x > 255.0)
		x = 255.0;
	r.b = (int)x;
	return (r);
}

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
