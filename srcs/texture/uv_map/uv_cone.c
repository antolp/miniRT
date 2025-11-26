/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_cone.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 04:49:31 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//init_cone_uv_vars():
//in this file because of norm
void	init_cyl_uv_vars(t_cyl_uv_vars *v, t_object *obj)
{
	v->cyl = (t_cylinder *)obj->shape;
	v->axis = vec_normalize(v->cyl->axis);
	build_basis_from_normal(v->axis, &v->tangent, &v->bitangent);
	v->eps_h = fmax(1e-6, 5e-4 * v->cyl->height);
	v->eps_r = fmax(1e-6, 5e-4 * v->cyl->radius);
}

//init_cone_uv_vars():
//Precomputes all data needed for cone UVs in one place.
//Builds axis A (unit), an orthonormal basis (T,B), offsets from apex,
//axial coordinate s = dot(P - apex, A), tolerances (eps_h, eps_r),
//base center (apex + A*height) and base radius (height * tan(angle)).
//
//if s < 0 hit lies behind the apex along A, exits (should never happen)
//for norm
static bool	init_cone_uv_vars(t_object *obj, t_vec3 *hit, t_cone_uv_vars *v)
{
	if (!obj || !hit || !v)
		return (false);
	v->cone = (t_cone *)obj->shape;
	v->axis = vec_normalize(v->cone->axis);
	build_basis_from_normal(v->axis, &v->tangent, &v->bitangent);
	v->hit_p = *hit;
	v->d = vec_sub(v->hit_p, v->cone->apex);
	v->s = vec_dot(v->d, v->axis);
	if (v->s < -1e-9)
		return (false);
	v->eps_h = fmax(1e-6, 5e-4 * v->cone->height);
	v->eps_r = fmax(1e-6, 5e-4 * v->cone->height);
	v->base_c = vec_add(v->cone->apex, vec_mul(v->axis, v->cone->height));
	v->r_base = v->cone->height * tan(v->cone->angle);
	return (true);
}

//cone_try_side():
//Maps a hit on the cone’s lateral surface to UV.
//
//first, compute off-axis vector and local radius:
//	w = d - A*s   (remove axial component), rlen = |w|,
//	r_here = s * tan(angle)  (expected cone radius at height s).
//
//then, check if we’re within the side surface :
//	s in [-eps_h, height+eps_h] and |rlen - r_here| ≤ eps_r
//		(unless r_here ~= 0 near apex).
//Get a stable radial direction rdir:
//	if rlen>0 -> rdir = w / rlen; else use T as fallback (apex).
//
//Angle around the cone for U:
//	theta = atan2( dot(rdir,B), dot(rdir,T) )  -> full wrap [-pi,pi].
//	u = (theta + pi) / (2pi)  -> map to [0,1] then wrap01(u).
//
//Axial fraction for V:
//	vcoord = s / height  -> map bottom->top to [0,1], then clamp01 to be safe.
static bool	cone_try_side(t_cone_uv_vars *v, t_vec2 *out_uv)
{
	t_vec3	rdir;
	double	theta;
	double	u;
	double	vcoord;

	v->w = vec_sub(v->d, vec_mul(v->axis, v->s));
	v->rlen = vec_length(v->w);
	v->r_here = v->s * tan(v->cone->angle);
	if (v->s < -v->eps_h || v->s > v->cone->height + v->eps_h)
		return (false);
	if (fabs(v->rlen - v->r_here) > v->eps_r && v->r_here > v->eps_r)
		return (false);
	if (v->rlen > 0.0)
		rdir = vec_mul(v->w, 1.0 / v->rlen);
	else
		rdir = v->tangent;
	theta = atan2(vec_dot(rdir, v->bitangent), vec_dot(rdir, v->tangent));
	u = (theta + M_PI) / (2.0 * M_PI);
	vcoord = v->s / v->cone->height;
	out_uv->x = wrap01(u);
	out_uv->y = clamp01(vcoord);
	return (true);
}

//cone_try_base():
//Checks if hit point P lies on the cone's base disk, then maps it to UV
//the checking is just like in the intersection logic, so it's redundant
//
//first, test if point on the base plane
//	db = P - baseC  (vector from base center to hit point)
// 	ht = |dot(db, A)| gives us how far we are off the base plane
//		along its normal A
//	If ht =< eps_h, point P is on the base plane. 
//
//then, test if point inside the base disk
//	r_xy = sqrt(|db|^2 - ht^2) is the in-plane distance from the center
//		(that's Pythagorea)
//	(remove the height part). Must be =< r_base + eps_r to be inside the disk.
//
//get 2D coords on that plane.
//	x = dot(db, T),  y = dot(db, B)  -> coordinates in the plane’s (T,B) axes,
//	centered at the base center (same as plane uv mapping).
//
//finally convert those to UV in [0,1].
//	The disk fits in a square of size 2*r_base around (0,0)
//	u = 0.5 + x / (2*r_base),  v = 0.5 + y / (2*r_base), then clamp to [0,1].
static bool	cone_try_base(t_cone_uv_vars *v, t_vec2 *out_uv)
{
	t_vec3	db;
	double	ht;
	double	r_xy;
	double	x;
	double	y;

	db = vec_sub(v->hit_p, v->base_c);
	ht = fabs(vec_dot(db, v->axis));
	if (ht > v->eps_h)
		return (false);
	r_xy = sqrt(vec_dot(db, db) - ht * ht);
	if (r_xy > v->r_base + v->eps_r)
		return (false);
	x = vec_dot(db, v->tangent);
	y = vec_dot(db, v->bitangent);
	out_uv->x = clamp01(0.5 + x / (2.0 * v->r_base));
	out_uv->y = clamp01(0.5 + y / (2.0 * v->r_base));
	return (true);
}

//init once try side then base
bool	get_uv_cone(t_object *obj, t_vec3 *hit, t_vec2 *out_uv)
{
	t_cone_uv_vars	v;

	if (!init_cone_uv_vars(obj, hit, &v))
		return (false);
	if (cone_try_side(&v, out_uv))
		return (true);
	if (cone_try_base(&v, out_uv))
		return (true);
	return (false);
}
