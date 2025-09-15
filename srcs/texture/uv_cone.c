/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_cone.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//init_cone_uv_vars():
//in this file because of norm
void	init_cyl_uv_vars(t_cyl_uv_vars *v, t_object *obj)
{
	v->cyl = (t_cylinder *)obj->shape;
	v->A = vec_normalize(v->cyl->axis);
	build_basis_from_normal(v->A, &v->T, &v->B);
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
	v->A = vec_normalize(v->cone->axis);
	build_basis_from_normal(v->A, &v->T, &v->B);
	v->P = *hit;
	v->d = vec_sub(v->P, v->cone->apex);
	v->s = vec_dot(v->d, v->A);
	if (v->s < -1e-9)
		return (false);
	v->eps_h = fmax(1e-6, 5e-4 * v->cone->height);
	v->eps_r = fmax(1e-6, 5e-4 * v->cone->height);
	v->baseC = vec_add(v->cone->apex, vec_mul(v->A, v->cone->height));
	v->r_base = v->cone->height * tan(v->cone->angle);
	return (true);
}

//cone_try_side():
//Computes UV on the lateral surface of the cone.
//Projects the hit point around the axis and maps to [0,1]x[0,1].
//U is the wrap-around angle, V is axial fraction from apex.
//
//key idea :
//  The cone radius grows linearly along the axis: r_here = s * tan(angle).
//  The off-axis (radial) direction is w = (P - apex) - A * s.
//  Angle around the cone is atan2(dot(rdir, B), dot(rdir, T)).
//
//Math steps :
//  s = dot(P - apex, A)         -> axial distance from apex
//  w = (P - apex) - A*s         -> radial vector off the axis
//  rlen = |w|                    -> actual radial distance at P
//  r_here = s * tan(angle)       -> expected radial distance on cone
//  near the sheet if |rlen - r_here| <= eps_r (or near apex where r_here≈0)
//  rdir = normalize(w) (fallback to T near apex to avoid NaN)
//  theta = atan2(dot(rdir,B), dot(rdir,T))   -> full 360° angle
//
//UV mapping :
//  u = (theta + pi) / (2*pi)     -> [-pi,pi] to [0,1]
//  v = s / height                -> apex→base mapped to [0,1]
static bool	cone_try_side(t_cone_uv_vars *v, t_vec2 *out_uv)
{
	t_vec3	rdir;
	double	theta;
	double	u;
	double	vcoord;

	v->w = vec_sub(v->d, vec_mul(v->A, v->s));
	v->rlen = sqrt(vec_dot(v->w, v->w));
	v->r_here = v->s * tan(v->cone->angle);
	if (v->s < -v->eps_h || v->s > v->cone->height + v->eps_h)
		return (false);
	if (fabs(v->rlen - v->r_here) > v->eps_r && v->r_here > v->eps_r)
		return (false);
	if (v->rlen > 0.0)
		rdir = vec_mul(v->w, 1.0 / v->rlen);
	else
		rdir = v->T;
	theta = atan2(vec_dot(rdir, v->B), vec_dot(rdir, v->T));
	u = (theta + M_PI) / (2.0 * M_PI);
	vcoord = v->s / v->cone->height;
	out_uv->x = wrap01(u);
	out_uv->y = clamp01(vcoord);
	return (true);
}

/* base (disk) UVs: project on (T,B) plane and normalize to [0,1]^2 */
static bool	cone_try_base(t_cone_uv_vars *v, t_vec2 *out_uv)
{
	t_vec3	db;
	double	ht;
	double	r_xy;
	double	x;
	double	y;

	db = vec_sub(v->P, v->baseC);
	ht = fabs(vec_dot(db, v->A));
	if (ht > v->eps_h)
		return (false);
	r_xy = sqrt(vec_dot(db, db) - ht * ht);
	if (r_xy > v->r_base + v->eps_r)
		return (false);
	x = vec_dot(db, v->T);
	y = vec_dot(db, v->B);
	out_uv->x = clamp01(0.5 + x / (2.0 * v->r_base));
	out_uv->y = clamp01(0.5 + y / (2.0 * v->r_base));
	return (true);
}

/* entry: init once, try side then base */
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
