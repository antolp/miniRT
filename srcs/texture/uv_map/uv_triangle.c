/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uv_triangle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 04:57:07 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	build_triangle_basis(const t_triangle *tr, t_vec3 *U, t_vec3 *V)
{
	t_vec3	e0;
	t_vec3	e1;
	t_vec3	normal;
	double	len;

	e0 = vec_sub(tr->p1, tr->p0);
	len = vec_length(e0);
	e0 = vec_mul(e0, 1.0 / len);
	e1 = vec_sub(tr->p2, tr->p0);
	normal = vec_cross(e0, e1);
	len = vec_length(normal);
	normal = vec_mul(normal, 1.0 / len);
	*U = e0;
	*V = vec_cross(normal, *U);
	len = vec_length(*V);
	*V = vec_mul(*V, 1.0 / len);
	return (1);
}

//barycentric()
//Computes the barycentric coordinates of a point P with respect to the triangle
//(p0, p1, p2). Outputs (b0, b1, b2) such that:
//	P ~= b0*p0 + b1*p1 + b2*p2   and   b0 + b1 + b2 = 1
//Returns 0 if the triangle is "invalid" (area ~= 0), 1 on success.
//
//Why barycentrics here:
//In get_uv_triangle(), FIT/FIT_OPPOSITE modes map the triangle to a unit
//right triangle in UV space by assigning vertices:
//	p0 -> (0,0),  p1 -> (1,0),  p2 -> (0,1)
//Under this mapping, a point’s UV becomes:
//	(u, v) = (b1, b2)
//	(and optionally flipped for FIT_OPPOSITE, allowing
//		full image area mapping onto a square formed by two triangles)
//This most importantly preserves straight lines !
//
//Math outline (dot-product formulation; stable, no trig):
//Let
//    v0 = p1 - p0
//    v1 = p2 - p0
//    v2 = P  - p0
//Build the Gram matrix and right-hand side via dot products:
//    d00 = v0·v0       d01 = v0·v1       d11 = v1·v1
//    d20 = v2·v0       d21 = v2·v1
//The 2×2 system for (b1, b2) solves:
//    [d00 d01][b1] = [d20]
//    [d01 d11][b2]   [d21]
//Its determinant is:
//    den = d00*d11 - d01*d01
//If den ≈ 0 the triangle is invalid (colinear vertices) -> return 0.
//Otherwise:
//    b1 = (d11*d20 - d01*d21) / den
//    b2 = (d00*d21 - d01*d20) / den
//    b0 = 1 - b1 - b2
//
//https://www.scratchapixel.com/lessons/3d-basic-rendering/
//ray-tracing-rendering-a-triangle/barycentric-coordinates.html
//https://en.wikipedia.org/wiki/Gram%E2%80%93Schmidt_process
static int	barycentric(const t_triangle *tr, t_vec3 P, t_bary_out *out)
{
	t_vec3	v[3];
	double	d[5];
	double	den;

	v[0] = vec_sub(tr->p1, tr->p0);
	v[1] = vec_sub(tr->p2, tr->p0);
	v[2] = vec_sub(P, tr->p0);
	d[0] = vec_dot(v[0], v[0]);
	d[1] = vec_dot(v[0], v[1]);
	d[2] = vec_dot(v[1], v[1]);
	d[3] = vec_dot(v[2], v[0]);
	d[4] = vec_dot(v[2], v[1]);
	den = d[0] * d[2] - d[1] * d[1];
	out->b1 = (d[2] * d[3] - d[1] * d[4]) / den;
	out->b2 = (d[0] * d[4] - d[1] * d[3]) / den;
	out->b0 = 1.0 - out->b1 - out->b2;
	return (1);
}

// p1 -> (1,0)
// p2 -> (0,1)
static bool	tri_uv_fit(const t_triangle *tr, t_vec3 *hit, t_vec2 *out_uv)
{
	t_bary_out	bc;

	if (barycentric(tr, *hit, &bc) == 0)
		return (false);
	out_uv->x = bc.b1;
	out_uv->y = bc.b2;
	return (true);
}

static bool	tri_uv_ortho(const t_triangle *tr, t_vec3 *hit, t_vec2 *out_uv)
{
	t_vec3	u;
	t_vec3	v;
	t_vec3	d;

	if (build_triangle_basis(tr, &u, &u) == 0)
		return (false);
	d = vec_sub(*hit, tr->p0);
	out_uv->x = vec_dot(d, u);
	out_uv->y = vec_dot(d, v);
	return (true);
}

//get_uv_triangle():
//Computes UVs for a triangle according to defined uv_mode
//Modes:
//	- TRI_UV_FIT:
//		Fills the triangle with the texture using barycentric coords.
//		Vertex mapping: p0 -> (0,0), p1 -> (1,0), p2 -> (0,1).
//		Result: (u,v) = (b1, b2).
//	- TRI_UV_FIT_OPPOSITE:
//		Same as FIT, then mirrored to the complementary half: (u,v) = (1-u, 1-v)
//		to allow displaying an image using two triangles.
//	- Otherwise (ORTHO):
//		Projects the hit onto the triangle’s local (U,V) basis (no stretch),
//		preserving local lengths for a more “orthonormal” mapping.
//		(original mode, display a non-scaled "grid" of texture pattern)
//Returns false on invalid inputs or barycentric failure.
bool	get_uv_triangle(t_object *obj, t_vec3 *hit, t_vec2 *out_uv)
{
	t_triangle	*tr;

	if (obj == NULL || hit == NULL || out_uv == NULL)
		return (false);
	tr = (t_triangle *)obj->shape;
	if (tr->uv_mode == TRI_UV_FIT || tr->uv_mode == TRI_UV_FIT_OPPOSITE)
	{
		if (!tri_uv_fit(tr, hit, out_uv))
			return (false);
		if (tr->uv_mode == TRI_UV_FIT_OPPOSITE)
		{
			out_uv->x = 1.0 - out_uv->x;
			out_uv->y = 1.0 - out_uv->y;
		}
		return (true);
	}
	return (tri_uv_ortho(tr, hit, out_uv));
}
