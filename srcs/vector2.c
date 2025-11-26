/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 04:41:49 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 05:05:38 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//returns the dot product of vectors a and b.
//it's a scalar value defined as: a.x * b.x + a.y * b.y + a.z * b.z
//extremely useful when dealing witgh angle calculations, projections, lighting 
//
//Let two unit vectors A and B :
//A . B = cos(delta), where delta is the angle between them.
//
// Quick tests with dot:
// -orthogonal: |a . b| ~= 0 
// -parallel: |a . b| ~= |a|*|b| (or ~= 1 if both unit vectors).
// -same hemisphere relative to normal n: A.N >= 0. (if both unit vectors)
//
//it measures how "aligned" the two vectors are
//If the dot product is:
//- Positive	-> vectors point roughly in the same direction
//- Negative	-> vectors point in opposite directions
//- Zero		-> vectors are perpendicular
//
//https://en.wikipedia.org/wiki/Dot_product
double	vec_dot(t_vec3 a, t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

//returns the cross product of vectors a and b.
//the result is a new vector that is perpendicular to both a and b.
//
//extremely useful to compute surface normals,
//build orthogonal axes (e.g. for camera orientation),
//among other things
//
//mathematically, defined as:
//	(a.y * b.z - a.z * b.y,
//	 a.z * b.x - a.x * b.z,
//	 a.x * b.y - a.y * b.x)
//
//the resulting vector’s length equals the area of the parallelogram
//spanned by a and b. If the result is (0, 0, 0), the vectors are parallel !!
//
//	https://en.wikipedia.org/wiki/Cross_product
t_vec3	vec_cross(t_vec3 a, t_vec3 b)
{
	t_vec3	cross;

	cross = (t_vec3)
	{
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
	return (cross);
}

//returns the normalized (unit) form of vector v
//a unit vector of v has the same direction but a length of 1
t_vec3	vec_normalize(t_vec3 v)
{
	double	len;

	len = vec_length(v);
	if (len == 0)
		return ((t_vec3){0, 0, 0});
	return ((t_vec3){v.x / len, v.y / len, v.z / len});
}

//normalize with a fallback if vec invalid, instead of null vec
//checking 0 (or eps) and checking fallback was redundant
//
//ishould change other vec_normalize calls to vec_safe_normalize if coherent
//(especially in intersection logic)
t_vec3	vec_safe_normalize(t_vec3 v, t_vec3 fallback)
{
	double	l;

	l = vec_length(v);
	if (l <= DBL_EPSILON)
		return (fallback);
	return (vec_mul(v, 1.0 / l));
}

//Computes the reflection of a direction vector across a surface normal.
//Both dir and normal are assumed to be normalized.
//
//uses the formula: R = D - 2 * (D . N) * N
//where :
//	- D is the incoming direction vector (comming from the cam hitting 
//	  the surface)
//	- N is the surface normal vector at the point of contact (normalized)
//the formula reflects D across the plane defined by N, like light 
//bouncing off a mirror
//useful to simulate reflection, and blinn-phong spec highlight
//drawing it on paper makes the maths obvious
//	https://www.fabrizioduroni.it/blog/post/2017/08/25/
//	how-to-calculate-reflection-vector
//	https://www.youtube.com/watch?v=naaeH1qbjdQ
t_vec3	vec_reflect(t_vec3 dir, t_vec3 normal)
{
	return (vec_sub(dir, vec_mul(normal, 2.0 * vec_dot(dir, normal))));
}
