/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//intersect_sphere()
//If the ray intersects the sphere:
//  - Computes the smallest valid intersection distance t
//  - Writes it into *t and returns true
//Else:
//  - Returns false
//
//Explanation :
//A ray intersects with a sphere if at least one point in the ray is also
//a point in the sphere. There can be 0, 1 (ray tangent to the sphere) or 
//2 intersections (ray goes through the sphere).
//
//Sphere is defined by:
//	- a center point C
//	- a radius r
//Sphere equation :
//	|P - C|² = r²
//meaning any point P on the sphere's surface is exactly 1 radius units from 
//its center.
//(PS :	|P - C| = r is equivalent, it may be much clearer, "A sphere is all
//		points at distance r from center point C")
//
//Now! substitution of P(t) point of intersection in sphere equation :
//	|(O + tD) - C|² = r²
//let's difine L = O - C (vector from sphere center to ray origin)
//refactoring :
//  |L + t * D|² = r²
//dot product expansion (just like in plane intersection):
//	(L + t * D) . (L + t * D) = r²
//	(D . D)t² + 2(L . D)t + (L . L - r²) = 0
//we get a quadratic (2nd degree polynomial) in the form: 
//	a * t² + b * t + c = 0
//where :
//	- a = D . D
//	- b = 2(L . D)
//	- c = L . L - r²
//
//solve it using the discriminant:
//	θ = b² - 4ac
//	- if θ < 0 -> no (real) solution, no intersection
//	- if θ = 0 -> ray tangent to the sphere, one intersection
//	- if θ > 0 -> two intersections (enter and exit the sphere)
//
//We can then finally get the value of t from the determinant using the
//quadratic formula : t = (-b -/+ sqrt(d)) / (2a);
//
////Practical example in 2D :
//        Y+
//         |
//         |  o     <-
//         |   \                    .
//        _|_   \   .               .
//    ~'`  |  `'~P(t0) <--------  ray origin (o)
//, `      |       \,           |
//         |        \ ,         |
//         |         \,         |
//         O----------\|        |
//         | r=4      ,\        |
//.        |         P(t1) <--- the two intersections
// ._      |      _.     \          .
// ___'' -~|~- ''_________\________> X+
//         |               \         .
//         |  (scale : 1x = 3 chars, 1y = 1 char)
//        Y-			
//	o = 1, 11;
//	D = 1/3, -1;
//	O = 0, 4;
//	r = 4;
//
//	from the formulas :
//	L = (1, 11) - (0, 4) = (1, 7)
//
//	a = (1/3, -1) . (1/3, -1)
//	  = (1/3)² + (-1)² 
//	  = 1/9 + 1 
//	  = 10/9
//
//	b = 2 * ((1, 7) . (1/3, -1))
//	  = 2 * [(1)*(1/3) + 7*(-1)] 
//	  = 2 * (1/3 - 7) 
//	  = -40/3
//
//	c = (1, 7) . (1, 7) - 16
//	  = (1² + 7²) - 16 
//	  = (1 + 49) - 16 
//	  = 34
//
//	now, for the discriminant :
//	θ = b² - 4ac
//		  = (-40/3)² - (4 * (10/9) * 34)
//		  = (1600 / 9) - (1360 / 9)
//		  = 80 / 3
//
//	θ positive, there's indeed two intersections !!	
//	in advance :
//		sqrt(80/3) ≈ 5.164
//		2a = 2 * (10/9) = 20 / 9
//	Now compute two solutions (or scalar):
//	t0 = (-b - sqrt(θ)) / 2a
//	   = (40/3 - 5.164) / (20/9)
//	   ≈ 3.675
//
//	t1 = (-b + sqrt(θ)) / 2a
//	   = (13.33 + 5.164) / (20/9)
//	   ≈ 8.325
//	The ray intersects the sphere at position:
//	t0 ≈ 3.675 (entry) and t1 ≈ 8.325 (exit) on the ray.
//
//	the coordinates of these points are :
//	P(t0) = (1, 11) + (1/3 * 3.675, -1 * 3.675)
//		  = (2.225, 7.325)
//	and most importantly :
//	P(t1) = (1, 11) + (1/3 * 8.325, -1 * 8.325)
//		  = (3.775, 2.675)
//
//	These values do seem correct according to our ascii drawing !
//	(Of course, during the rendering we don't compute the exit of the ray)
//
//ressources :
//	https://en.wikipedia.org/wiki/Quadratic_equation (en)
//	https://fr.wikipedia.org/wiki/%C3%89quation_du_second_degr%C3%A9 (fr)
//	https://www.scratchapixel.com/lessons/3d-basic-rendering/
//	minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection.html
//	(analytic solution)
bool	intersect_sphere(t_object *obj, t_ray *ray, double *t)
{
	t_quad		q;
	t_vec3		l;
	t_sphere	*s;

	s = (t_sphere *)obj->shape;
	l = vec_sub(ray->origin, s->center);
	q.a = vec_dot(ray->direction, ray->direction);
	q.b = 2.0 * vec_dot(l, ray->direction);
	q.c = vec_dot(l, l) - s->radius * s->radius;
	q.d = q.b * q.b - 4 * q.a * q.c;
	if (q.d < 0)
		return (false);
	q.sqrt_d = sqrt(q.d);
	q.t0 = (-q.b - q.sqrt_d) / (2 * q.a);
	q.t1 = (-q.b + q.sqrt_d) / (2 * q.a);
	if (q.t0 > 1e-4)
		*t = q.t0;
	else if (q.t1 > 1e-4)
		*t = q.t1;
	else
		return (false);
	return (true);
}

//normal vector defined by direction from center of the sphere to hit_point
void	get_normal_sphere(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal)
{
	t_sphere	*sphere;

	sphere = (t_sphere *)obj->shape;
	*out_normal = vec_normalize(vec_sub(*hit_point, sphere->center));
}
