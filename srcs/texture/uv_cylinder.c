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


//get_uv_cylinder_side(): TROP LONG AAGHGGGGGHHHHH jen ai marre
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
static bool	get_uv_cylinder_side(t_cyl_uv_vars *v, t_vec3 *hit, t_vec2 *out_uv)
{
	t_vec3	d;
	t_vec3	rdir;
	double	theta;
	double	u;
	double	vcoord;

	d = vec_sub(*hit, v->cy->center);
	v->h = vec_dot(d, v->A);
	v->rv = vec_sub(d, vec_mul(v->A, v->h));
	v->rlen = sqrt(vec_dot(v->rv, v->rv));
	v->eps = fmax(1e-6, 5e-4 * v->cy->radius);
	if (v->h < -v->eps)
		return (false);
	if (v->h > v->cy->height + v->eps)
		return (false);
	if (fabs(v->rlen - v->cy->radius) > v->eps)
		return (false);
	if (v->rlen > 0.0)
		rdir = vec_mul(v->rv, 1.0 / v->rlen);
	else
		rdir = v->T;
	theta = atan2(vec_dot(rdir, v->B), vec_dot(rdir, v->T));
	u = (theta + M_PI) / (2.0 * M_PI);
	vcoord = v->h / v->cy->height;
	out_uv->x = wrap01(u);
	out_uv->y = clamp01(vcoord);
	return (true);
}

//Projects a 3D point lying on a cap plane into 2D (UV) using (T, B).
//Scales and offsets so the disk is centered in the [0,1]^2 UV space.
//CAP_UV_SCALE initially 2
static bool	get_uv_cylinder_cap(t_cyl_uv_vars *v, t_vec3 d, t_vec2 *out_uv)
{
	double	x;
	double	y;

	x = vec_dot(d, v->T);
	y = vec_dot(d, v->B);
	out_uv->x = clamp01(0.5 + x / (CAP_UV_SCALE * v->cy->radius));
	out_uv->y = clamp01(0.5 + y / (CAP_UV_SCALE * v->cy->radius));
	return (true);
}

//Handles the top cap of the cylinder.
//Projects the hit point onto the cap plane and maps to UV.
//first check if hit point is on cap plane
//	dot(dt, A);
//then checks if point is inside radius :
// |dt|^2 - ht^2 <= radius^2
static bool	get_uv_cylinder_top(t_cyl_uv_vars *v, t_vec3 *hit, t_vec2 *out_uv)
{
	t_vec3	Ct;
	t_vec3	dt;
	double	ht;
	double	rt;
	t_vec2	eps;

	Ct = vec_add(v->cy->center, vec_mul(v->A, v->cy->height));
	dt = vec_sub(*hit, Ct);
	ht = vec_dot(dt, v->A);
	eps.x = fmax(1e-6, 5e-4 * v->cy->height);
	if (fabs(ht) > eps.x)
		return (false);
	rt = sqrt(vec_dot(dt, dt) - ht * ht);
	eps.y = fmax(1e-6, 5e-4 * v->cy->radius);
	if (rt > v->cy->radius + eps.y)
		return (false);
	return (get_uv_cylinder_cap(v, dt, out_uv));
}

//Computes UV coordinates on the curved side of the cylinder.
//Projects the hit point around the axis, and maps it to [0, 1] range,
//then builds a radial vector and calculates the angle theta around the axis
//U is derived from angle, V from height along the axis.
bool	get_uv_cylinder(t_object *obj, t_vec3 *hit, t_vec2 *out_uv)
{
	t_cyl_uv_vars	v;
	t_vec3			d;
	double			h;
	double			eps_h;
	double			eps_r;

	if (!obj || !hit || !out_uv)
		return (false);
	v.cy = (t_cylinder *)obj->shape;
	v.A = vec_normalize(v.cy->axis);
	build_basis_from_normal(v.A, &v.T, &v.B);
	if (get_uv_cylinder_side(&v, hit, out_uv))
		return (true);
	d = vec_sub(*hit, v.cy->center);
	h = vec_dot(d, v.A);
	eps_h = fmax(1e-6, 5e-4 * v.cy->height);
	eps_r = fmax(1e-6, 5e-4 * v.cy->radius);
	if (fabs(h) <= eps_h && vec_length(d) <= v.cy->radius + eps_r)
		return (get_uv_cylinder_cap(&v, d, out_uv));
	return (get_uv_cylinder_top(&v, hit, out_uv));
}
