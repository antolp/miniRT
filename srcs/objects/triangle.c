/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:57:43 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//intersect_triangle()
//tests ray-triangle intersection using the Möller–Trumbore algorithm
//
//Triangle is defined by:
//  - vertices p0, p1, p2
//We want to solve for t, u, v in the equation:
//  O + t * D = p0 + u * (p1 - p0) + v * (p2 - p0)
//Here, (u, v) are the barycentric coordinates of the intersection point
//relative to the triangle, and t is the distance from the ray origin to
//that point along the ray direction.
//(en.wikipedia.org/wiki/Barycentric_coordinate_system)
//
//Starting from the ray/triangle intersection equation:
//  O + t*D = p0 + u*(p1 - p0) + v*(p2 - p0)
//Define edges:
//  edge1 = p1 - p0
//  edge2 = p2 - p0
//Substituting edges:
//  O + t*D = p0 + u*edge1 + v*edge2
//Move O to the right-hand side:
//  t*D = (p0 - O) + u*edge1 + v*edge2
//Finally move u*edge1 and v*edge2 to the left-hand side:
//  t*D - u*edge1 - v*edge2 = p0 - O
//
//This is a 3×3 linear system in (t, u, v).
//(this just means we have thre unkown t, u, v and three equation that
//come from x, y, and z components of the ray–triangle intersection equation)
//Möller–Trumbore usually solves it using Cramer's rule, computing determinants
//via scalar triple products (dot of a cross product). i won't explain this
//because i don't understand it. lol
//(en.wikipedia.org/wiki/Cramer%27s_rule)
//
//edge1	= p1 - p0
//edge2	= p2 - p0
//pvec	= D × edge2           -> used in det(A) and to get u
//det	= edge1 ⋅ pvec        -> det(A), the main denominator
//inv_det = 1 / det           -> factor to avoid repeated divisions
//tvec	= O - p0              -> vector from p0 to ray origin
//u		= (tvec ⋅ pvec) * inv_det
//		  barycentric coord along edge1 direction. u in [0,1] for hit
//qvec	= tvec × edge1
//v     = (D ⋅ qvec) * inv_det
//		  barycentric coord along edge2 direction. v in [0,1] and u + v ≤ 1
//t_cand = (edge2 ⋅ qvec) * inv_det
//		  distance along ray. must be > 0 and less than current closest hit
//
//If det is near zero, ray is parallel to triangle plane -> no hit.
//If u or v are out of range, or u+v > 1, point lies outside triangle.
//Otherwise, t_cand is a valid hit distance and we update t.
//
//shamelessly copied C++ implementation from :
//en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
//good ressource to understand the overall idea :
//www.scratchapixel.com/lessons/3d-basic-rendering/
//ray-tracing-rendering-a-triangle/
//moller-trumbore-ray-triangle-intersection.html
bool	intersect_triangle(t_object *obj, t_ray *ray, double *t)
{
	t_triangle	*tri;
	t_tri_vars	v;

	tri = (t_triangle *)obj->shape;
	v.edge1 = vec_sub(tri->p1, tri->p0);
	v.edge2 = vec_sub(tri->p2, tri->p0);
	v.pvec = vec_cross(ray->direction, v.edge2);
	v.det = vec_dot(v.edge1, v.pvec);
	if (fabs(v.det) < 1e-6)
		return (false);
	v.inv_det = 1.0 / v.det;
	v.tvec = vec_sub(ray->origin, tri->p0);
	v.u = vec_dot(v.tvec, v.pvec) * v.inv_det;
	if (v.u < 0.0 || v.u > 1.0)
		return (false);
	v.qvec = vec_cross(v.tvec, v.edge1);
	v.v = vec_dot(ray->direction, v.qvec) * v.inv_det;
	if (v.v < 0.0 || v.u + v.v > 1.0)
		return (false);
	v.tcand = vec_dot(v.edge2, v.qvec) * v.inv_det;
	if (v.tcand < 1e-4 || v.tcand >= *t)
		return (false);
	*t = v.tcand;
	return (true);
}

//normal is vector perpendicular to a vector formed by any point on triangle
//take two 
void	get_normal_triangle(t_object *obj, t_vec3 *hit_point,
			t_vec3 *out_normal)
{
	t_triangle	*tri;
	t_vec3		e1;
	t_vec3		e2;
	t_vec3		n;

	(void)hit_point;
	tri = (t_triangle *)obj->shape;
	e1 = vec_sub(tri->p1, tri->p0);
	e2 = vec_sub(tri->p2, tri->p0);
	n = vec_cross(e1, e2);
	*out_normal = vec_normalize(n);
}
