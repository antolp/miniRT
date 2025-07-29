/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"


//intersect_plane()
//If ray interesect plane :
//	-write in t the scalar of interesction point, returns true.
//else :
//	-returns false.
//
//Explanation :
//A ray interescts if angle from ray to the plane normal is < 90
//easily calculated through dot product.
//t scalar is then calculated by substituting P(t) 
//
//Important premise, P(t) mathematical function defining the ray :
//P(t) = O + t * D where :
//	O origin of rqy (camera)
//	D direction of ray
//	t scalar (again)
//a ray is really just an origin and a direction.
//an easier way to imagine this simple formula would be to consider
//an affine (linear) function in the form of f(x) = yx;
//then, a ray can be seen as simple affine function in 3D space:
//instead of working with numbers on a line, we use 3D vectors
//as t changes, P(t) moves along the ray starting from O in the direction D
//
//Plane defined by :
//	- a point Po
//	- a normal vector N
//Plane equation :
//dot(P(t) - Po, N) = 0 where :
//	Po is any point on plane (plane->point for ex)
//	P(t) is a point on the ray (t scalar, e.g. how far in ray vector)
//	N normal vector
//Angle of vector formed by two points on plane an normal should be 90
//(dot product = 0)
//
//Now !! substitution of P(t) point of intersection in plane equation :
//dot((O + t * D - Po), N) = 0 
//refactoring (dot product is commutative) :
//dot((O - Po), N) + t * dot(D, N) = 0
//finally, solving for t :
//t = dot(P₀ - O, N) / dot(D, N)  -> that's our formula
//
//Practical example in 2D :
//        Y+
//        |
//        |             o     <- ray origin (o)
//        |            /
//        |           /
//        |          /
//        |         /
//        |        /
//        |       /
//        |      /
//        |     /
//        |    /
// -------+--P(t)----------------> X+ (plane at y = 0, normal (0, 1))
//        |   ^
//        |   |------- the intersection 
//        Y-
//  o  = 7, 10;
//  D  = -0.5, -1;
//  P0 = 0, 0;
//  N = 0, 1;
//
//	from the formula :
//  t = dot(P0 - O, N) / dot(D, N)
//  t = dot((-7, -10), (0, 1)) /dot((-0.5, -1), (0, 1))
//  t = -10 / -1
//  t= 10
//
//  we can then, in trace_ray(), get the hit point in the scene :
//  P(t) = O + t * D 
//  P(10) = (7, 10) + 10 × (-0.5, -1)
//  P(10) = (7, 10) + (-5, -10)
//  P(10) = (2, 0)
//
//  From the ascii drawing we can see that the intersection occurs at (2, 0) !!
bool	intersect_plane(t_object *obj, t_ray *ray, double *t)
{
	t_plane	*plane;
	double	denom;
	t_vec3	p_to_o;

	plane = (t_plane *)obj->shape;
	denom = vec_dot(plane->normal, ray->direction);
	if (fabs(denom) < 1e-6)
		return (false);
	p_to_o = vec_sub(plane->point, ray->origin);
	*t = vec_dot(p_to_o, plane->normal) / denom;
	if (*t < 0.001)
		return (false);
	return (true);
}

//plane normal is constant (self evident, it's a plane)
void	get_normal_plane(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal)
{
	t_plane	*plane;

	(void)hit_point;
	plane = (t_plane *)obj->shape;
	*out_normal = vec_normalize(plane->normal);
}
