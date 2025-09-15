/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_cylinder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

#define CAP_UV_SCALE 5

//prepare hit-dependent quantities: d, h, rv, rlen
static void	cyl_uv_prep_hit(t_cyl_uv_vars *v, t_vec3 *hit)
{
	v->d = vec_sub(*hit, v->cyl->center);
	v->h = vec_dot(v->d, v->A);
	v->rv = vec_sub(v->d, vec_mul(v->A, v->h));
	v->rlen = sqrt(vec_dot(v->rv, v->rv));
}

//get_uv_cylinder_side():
//Computes UV coordinates on the curved side of the cylinder.
//Projects the hit point around the axis, and maps it to [0, 1] range.
//Builds a radial vector and calculates the angle theta around the axis.
//U is derived from angle, V from height along the axis.
//
//key idea :
//	atan2(y, x) gives angle from X axis to vector (gives full 360deg angle) !!
//
//A is the axis of the cylinder (from bottom to top).
//T and B are orthonormal tangent vectors forming the base.
//The vector d is from the cylinder’s base to the hit point :
//	h = dot(d, A) gives the height coordinate (how far along the axis).
//	rv = d - A * h gives the radial vector from the axis to the hit point.
//	θ (theta) is the angle around the cylinder,
//	calculated with atan2(dot(rv, B), dot(rv, T))
//Converts to U: 
//	u = (theta + pi) / 2pi to map from [-pi, pi] -> [0, 1]
//Finally :
//	v = h / height maps axial position to [0, 1]
static bool	cylinder_try_side(t_cyl_uv_vars *v, t_vec3 *hit, t_vec2 *out_uv)
{
	t_vec3	rdir;
	double	theta;
	double	u;
	double	vcoord;

	cyl_uv_prep_hit(v, hit);
	if (v->h < -v->eps_h)
		return (false);
	if (v->h > v->cyl->height + v->eps_h)
		return (false);
	if (fabs(v->rlen - v->cyl->radius) > v->eps_r)
		return (false);
	if (v->rlen > 0.0)
		rdir = vec_mul(v->rv, 1.0 / v->rlen);
	else
		rdir = v->T;
	theta = atan2(vec_dot(rdir, v->B), vec_dot(rdir, v->T));
	u = (theta + M_PI) / (2.0 * M_PI);
	vcoord = v->h / v->cyl->height;
	out_uv->x = wrap01(u);
	out_uv->y = clamp01(vcoord);
	return (true);
}

//cylinder_try_one_cap():
//one cap test at center C: plane test + in-disk test + (T,B) projection
//third time im writing this
static bool	cylinder_try_one_cap(t_cyl_uv_vars *v, t_vec3 *hit,
	t_vec3 C, t_vec2 *out_uv)
{
	t_vec3	d;
	double	h;
	double	r_xy;
	double	x;
	double	y;

	d = vec_sub(*hit, C);
	h = vec_dot(d, v->A);
	if (fabs(h) > v->eps_h)
		return (false);
	r_xy = sqrt(vec_dot(d, d) - h * h);
	if (r_xy > v->cyl->radius + v->eps_r)
		return (false);
	x = vec_dot(d, v->T);
	y = vec_dot(d, v->B);
	out_uv->x = clamp01(0.5 + x / (CAP_UV_SCALE * v->cyl->radius));
	out_uv->y = clamp01(0.5 + y / (CAP_UV_SCALE * v->cyl->radius));
	return (true);
}

//cylinder_try_caps():
//Handles the bottom then top cap of the cylinder
static bool	cylinder_try_caps(t_cyl_uv_vars *v, t_vec3 *hit, t_vec2 *out_uv)
{
	t_vec3	C;
	bool	oui;

	C = v->cyl->center;
	oui = cylinder_try_one_cap(v, hit, C, out_uv);
	if (oui)
		return (true);
	C = vec_add(v->cyl->center, vec_mul(v->A, v->cyl->height));
	oui = cylinder_try_one_cap(v, hit, C, out_uv);
	if (oui)
		return (true);
	return (false);
}

//get_uv_cylinder():
//Computes UV coordinates on the curved side of the cylinder.
//Projects the hit point around the axis, and maps it to [0, 1] range,
//then builds a radial vector and calculates the angle theta around the axis
//U is derived from angle, V from height along the axis.
bool	get_uv_cylinder(t_object *obj, t_vec3 *hit, t_vec2 *out_uv)
{
	t_cyl_uv_vars	v;

	if (!obj || !hit || !out_uv)
		return (false);
	init_cyl_uv_vars(&v, obj);
	if (cylinder_try_side(&v, hit, out_uv))
		return (true);
	if (cylinder_try_caps(&v, hit, out_uv))
		return (true);
	return (false);
}
