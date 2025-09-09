/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	get_cap_info(t_cylinder *cyl, int i, t_vec3 *center, t_vec3 *normal)
{
	t_vec3	axis_n;

	axis_n = vec_normalize(cyl->axis);
	if (i == 0)
	{
		*center = cyl->center;                 // base center
		*normal = vec_mul(axis_n, -1.0);       // base normal points outward
	}
	else
	{
		*center = vec_add(cyl->center, vec_mul(axis_n, cyl->height)); // top center
		*normal = axis_n;                      // top normal
	}
}


//check_cap()
//the real ray-cap intersection check function
//
//Each cap is a flat disk defined by:x
//  - center point C (v->center)
//  - normal vector N (v->normal), oriented outwards
//  - radius r (same as the cylinder)
//
//We want to find a scalar t such that:
//  - The point P(t) = O + t * D lies on the cap's plane
//  - The point P(t) lies within a circle of radius r centered at C
//
//We start by computing denominator, which is the dot product between the ray
//direction D and the cap's normal N:
//  denom = D . N
//The dot product measures the alignment between the ray and the "plane" normal
//If denom is close to zero, D is nearly perpendicular to N, so the ray is
//parallel to the plane and cannot intersect it, we return false.
//
//If denom is not zero, we solve the ray-plane intersection:
//The cap lies on a plane, and any point P on that plane must satisfy:
//	(P - C) . N = 0
//Substitute with P = O + t * D:
//	(O + t * D - C) . N = 0
//Refactoring :
//	(O - C) . N + t * (D . N) = 0
//Solving for t:
//	t = -((O - C) . N) / (D . N)
//Equivalent to :
//	t = ((C - O) . N) / (D . N)		<---- line 79
//This gives us the value of t for which the ray intersects the cap's plane
//
//We now compute the point of intersection on the ray:
//	P(t) = O + t * D
//This is the point where the ray intersects the plane of the cap.
//	
//Next, we check if the point is inside the circular disk of the cap.
//We compute the squared distance between P(t) and the center C:
//	|P(t) - C|² = (P(t) - C) . (P(t) - C)
//If this squared distance is greater than r², the point lies outside
//the disk and is not a valid cap intersection !!
//
//If the point is inside the radius and t is positive,
//we update *t_hit and return true
static bool	check_cap_hit(t_ray *ray, t_cylinder *cyl, t_cap_vars *v)
{
	v->denom = vec_dot(ray->direction, v->normal);
	if (fabs(v->denom) < 1e-6)
		return (false);
	v->t = vec_dot(vec_sub(v->center, ray->origin), v->normal) / v->denom;
	if (v->t <= 1e-4 || v->t >= v->best_t)
		return (false);
	v->p = vec_add(ray->origin, vec_mul(ray->direction, v->t));
	if (vec_length_squared(vec_sub(v->p, v->center)) > cyl->radius * cyl->radius)
		return (false);
	v->best_t = v->t;
	v->best_p = v->p;
	v->found = true;
	return (true);
}


//intersect_cylinder_caps()
//tests the intersection between ray and the top and bottom "caps", circular faces
//either base (v.i == 0) or top face (v.i == 1)
//first checks base then top cap, if not intersection, check bottom cap
static bool	intersect_cylinder_caps(t_ray *ray, t_cylinder *cyl,
	double *t_hit, t_vec3 *hit_point)
{
	t_cap_vars	v;

	v.i = 0;
	v.found = false;
	v.best_t = *t_hit;
	while (v.i < 2)
	{
		get_cap_info(cyl, v.i, &v.center, &v.normal);
		check_cap_hit(ray, cyl, &v);
		v.i++;
	}
	if (!v.found)
		return (false);
	*t_hit = v.best_t;
	*hit_point = v.best_p;
	return (true);
}

//hit must lie between apex (0) and base (height)
//use dot product to compare vector projection
static bool	is_point_within_cylinder_height(t_vec3 point, t_cylinder *cyl)
{
	t_vec3	v;
	double	proj;

	v = vec_sub(point, cyl->center);
	proj = vec_dot(v, cyl->axis);
	return (proj >= 0.0 && proj <= cyl->height);
}

//helper for norm
static void	compute_quad(t_quad *q, t_cyl_side_vars v, double radius)
{
	q->a = vec_dot(v.d_perp, v.d_perp);
	q->b = 2.0 * vec_dot(v.d_perp, v.cto_perp);
	q->c = vec_dot(v.cto_perp, v.cto_perp) - radius * radius;
	q->d = q->b * q->b - 4.0 * q->a * q->c;
}

//intersect_infinite_cylinder_side()
//tests the intersection between ray and with the side of an infinite cylinder
//
//Variables are seperated between two structs, cylinder variables 
//and quadratic computation variables :
//	- v.c_to_o: vector from cylinder base center to ray origin
//	- v.d: ray direction
//	- v.axis: cylinder axis
//	- v.d_perp: component of ray direction perpendicular to the axis
//	- v.cto_perp: component of c_to_o vector perpendicular to the axis
//	- q: quadratic coefficients for solving intersection equation
//
//Explanation :
//A cylinder can be thought of as a set of points at a fixed distance (radius)
//from an segment (the cylinder axis from origin to height).
//A ray intersects with the side of a cylinder if at some value t, the point
//P(t) = O + tD lies exactly `radius` units from the axis line.
//this function assume the cylinder is infinite, we den clamp it to the height
//of the cylinder
//
//Cylinder is defined by:
//  - a center point C (typically the base center)
//  - a normalized axis vector V
//  - a radius r
//
//We want to find t such that the shortest distance between the point P(t)
//and the cylinder axis is exactly r. To do that we have:
//  - O = ray origin
//  - D = ray direction (normalized or not)
//  - L = O - C = vector from cylinder base to ray origin (v.c_to_o)
//We compute the components of D and L perpendicular to the cylinder axis:
//  - D_perp = D - (D . V) * V
//  - L_perp = L - (L . V) * V
//
//Why?
//The idea is that D_perp and L_perp describe the ray's projection in the
//plane perpendicular to the cylinder's axis, where the problem becomes
//similar to a 2D circle intersection.
//
//We then solve the following equation:
//  |L_perp + t * D_perp|² = r²
//Expanding this with the dot product:
//	(L_perp + t * D_perp) . (L_perp + t * D_perp) = r²
//refactoring :
//	(D_perp . D_perp) * t² + 2 * (D_perp . L_perp) * t + (L_perp . L_perp) = r²
//bringing r² to the left, have our quadratic equation !!
//  a * t² + b * t + c = 0
//where:
//  - a = D_perp . D_perp
//  - b = 2 * (D_perp . L_perp)
//  - c = L_perp . L_perp - r²
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
//(no drawing this time, may add one i drew on paper)
static bool	intersect_infinite_cylinder_side(t_ray *ray, t_cylinder *cyl,
	double *t_hit, t_vec3 *point)
{
	t_quad				q;
	t_cyl_side_vars		v;

	v.axis = cyl->axis;
	v.c_to_o = vec_sub(ray->origin, cyl->center);
	v.d = ray->direction;
	v.d_perp = vec_sub(v.d, vec_mul(v.axis, vec_dot(v.d, v.axis)));
	v.cto_perp = vec_sub(v.c_to_o, vec_mul(v.axis, vec_dot(v.c_to_o, v.axis)));
	compute_quad(&q, v, cyl->radius);
	if (q.d < 0.0)
		return (false);
	q.sqrt_d = sqrt(q.d);
	q.t0 = (-q.b - q.sqrt_d) / (2.0 * q.a);
	q.t1 = (-q.b + q.sqrt_d) / (2.0 * q.a);
	if (q.t0 > 1e-4 && q.t0 < *t_hit)
		*t_hit = q.t0;
	else if (q.t1 > 1e-4 && q.t1 < *t_hit)
		*t_hit = q.t1;
	else
		return (false);
	*point = vec_add(ray->origin, vec_mul(ray->direction, *t_hit));
	return (true);
}

//First check for intersection with caps. by definition, closest facing cap
//is in front of the side or "body" of the cylinder
//then, check for intersection with the side of the cylinder
//for side-ray intersection check, we check a mathematically infinite cylinder
//and if there's an intersection, we check if the ray intersects with
bool	intersect_cylinder(t_object *obj, t_ray *ray, double *t)
{
	t_cylinder	*cyl;
	t_vec3		point;
	double		t_temp;
	bool		hit;

	cyl = (t_cylinder *)obj->shape;
	hit = false;
	t_temp = *t;
	if (intersect_cylinder_caps(ray, cyl, &t_temp, &point))
	{
		*t = t_temp;
		hit = true;
	}
	if (intersect_infinite_cylinder_side(ray, cyl, &t_temp, &point))
	{
		if (is_point_within_cylinder_height(point, cyl) && t_temp < *t)
		{
			*t = t_temp;
			hit = true;
		}
	}
	return (hit);
}

//here, again, there's two cases to handle.
//if the intersection point is on the side of the cylider :
//	- normal is the vector that goes from the closest point on the axis to
//	  the intersection point on the cylinder
//if the intersection point is on either cap :
//	- normal is the axis vector if top cap, -axis vector if bottom cap
//
//first, we compute b the vector that goes from base center to hit point
//then we project v onto the cylinder axis, "how far we are along axis vector"
//if proj == 0 OR v == height :
//	- we are on the caps, return axis or -axis.
//else :
//	- return vector from closestg point on axis to intersection
void	get_normal_cylinder(t_object *obj, t_vec3 *hit_point,
	t_vec3 *out_normal)
{
	t_cylinder	*cyl;
	t_vec3		v;
	t_vec3		proj_vec;
	double		proj;

	cyl = (t_cylinder *)obj->shape;
	v = vec_sub(*hit_point, cyl->center);
	proj = vec_dot(v, cyl->axis);
	if (proj <= 1e-4)
		*out_normal = vec_mul(cyl->axis, -1);
	else if (proj >= cyl->height - 1e-4)
		*out_normal = cyl->axis;
	else
	{
		proj_vec = vec_mul(cyl->axis, proj);
		*out_normal = vec_normalize(vec_sub(v, proj_vec));
	}
}
