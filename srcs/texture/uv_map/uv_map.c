/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_map.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 04:56:37 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//build_basis_from_normal():
//To map UV coordinates on the object's surface, we need an "orthonormal basis",
//so two vectors that are perpendicular to the normal, and between themselves,
//these vectors are the tangents (T) and bitangents (B) to the plane's surface
//We use the axis for cones and cylinders caps, normal for planes
//if d is too much similar to the up (y+) direction, fallback to left (x+)
void	build_basis_from_normal(t_vec3 a, t_vec3 *t, t_vec3 *b)
{
	t_vec3	up;
	double	d;

	up = (t_vec3){0.0, 1.0, 0.0};
	d = fabs(vec_dot(a, up));
	if (d > 0.999)
		up = (t_vec3){1.0, 0.0, 0.0};
	*t = vec_normalize(vec_cross(up, a));
	*b = vec_cross(a, *t);
}

//u = (atan2(z,x)+pi)/(2pi) and v = acos(y)/pi, using normalized local dir
//will write bigger comment eventually since the maths are pretty nice
//basically just returning an angle from hemisphere and equator of the sphere
//using atan2() and asin().
bool	get_uv_sphere(t_object *obj, t_vec3 *hit, t_vec2 *out_uv)
{
	t_sphere	*sp;
	t_vec3		p;
	double		len;
	double		u;
	double		v;

	if (obj == NULL || hit == NULL || out_uv == NULL)
		return (false);
	sp = (t_sphere *)obj->shape;
	p = vec_sub(*hit, sp->center);
	len = vec_length(p);
	if (len <= 0.0)
		return (false);
	p = vec_mul(p, 1.0 / len);
	u = 0.5 + atan2(p.z, p.x) / (2.0 * M_PI);
	u = wrap01(u);
	p.y = fmax(-1.0, fmin(1.0, p.y));
	v = 0.5 + asin(p.y) / M_PI;
	out_uv->x = u;
	out_uv->y = v;
	return (true);
}

//get_uv_plane():
//projects a hit point onto a plane's local orthonormal basis to get UV coords
bool	get_uv_plane(t_object *obj, t_vec3 *hit, t_vec2 *out_uv)
{
	t_plane		*pl;
	t_vec3		t;
	t_vec3		b;
	t_vec3		d;
	t_vec2		uv;

	if (obj == NULL || hit == NULL || out_uv == NULL)
		return (false);
	pl = (t_plane *)obj->shape;
	build_basis_from_normal(vec_normalize(pl->normal), &t, &b);
	d = vec_sub(*hit, pl->point);
	uv.x = vec_dot(d, t);
	uv.y = vec_dot(d, b);
	out_uv->x = uv.x;
	out_uv->y = uv.y;
	return (true);
}
