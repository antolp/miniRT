/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//hit must lie between apex (0) and base (height)
//use dot product to compare vector projection
static bool	is_point_within_cone_height(t_vec3 p, t_cone *cone)
{
	t_vec3	ap;
	double	proj;

	ap = vec_sub(p, cone->apex);
	proj = vec_dot(ap, cone->axis);
	if (proj < 0.0)
		return (false);
	if (proj > cone->height)
		return (false);
	return (true);
}

//simpler, non factorized version of the cylinder cap logic
//since there's only one cap to check
//p var local to the function
//should reuse check_cap() eventually, eh
static bool	intersect_cone_caps(t_ray *ray, t_cone *cone,
	double *t_hit, t_vec3 *hit_point)
{
	t_cone_cap_vars	v;
	t_vec3			p;

	v.center = vec_add(cone->apex, vec_mul(cone->axis, cone->height));
	v.normal = cone->axis;
	v.radius = cone->height * tan(cone->angle);
	v.denom = vec_dot(ray->direction, v.normal);
	if (fabs(v.denom) < 1e-6)
		return (false);
	v.t = vec_dot(vec_sub(v.center, ray->origin), v.normal) / v.denom;
	if (v.t < 1e-4 || v.t >= *t_hit)
		return (false);
	p = vec_add(ray->origin, vec_mul(ray->direction, v.t));
	if (vec_length_squared(vec_sub(p, v.center)) > v.radius * v.radius)
		return (false);
	*t_hit = v.t;
	*hit_point = p;
	return (true);
}

//helper for norm
static void	compute_quad(t_quad *q, t_cone_side_vars v)
{
	q->a = v.dv * v.dv - v.cos2 * vec_dot(v.d, v.d);
	q->b = 2.0 * (v.dv * v.cov - v.cos2 * vec_dot(v.d, v.co));
	q->c = v.cov * v.cov - v.cos2 * vec_dot(v.co, v.co);
	q->d = q->b * q->b - 4.0 * q->a * q->c;
	return;
}

//very similar to cylinder, except we use cos2 to compute
//the radius of ou cyliner in a give point on the axis
//
//no drawing for now, i have one drawn on my textbook
static bool	intersect_infinite_cone_side(t_ray *ray, t_cone *cone,
	double *t_hit, t_vec3 *point)
{
	t_cone_side_vars	v;
	t_quad				q;
	double				tcand;

	v.co = vec_sub(ray->origin, cone->apex);
	v.d = ray->direction;
	v.cos2 = pow(cos(cone->angle), 2.0);
	v.dv = vec_dot(v.d, cone->axis);
	v.cov = vec_dot(v.co, cone->axis);
	compute_quad(&q, v);
	if (q.d < 0.0)
		return (false);
	q.sqrt_d = sqrt(q.d);
	q.t0 = (-q.b - q.sqrt_d) / (2.0 * q.a);
	q.t1 = (-q.b + q.sqrt_d) / (2.0 * q.a);
	tcand = 1e30;
	if (q.t0 > 1e-4 && q.t0 < tcand)
		tcand = q.t0;
	if (q.t1 > 1e-4 && q.t1 < tcand)
		tcand = q.t1;
	if (tcand >= *t_hit)
		return (false);
	*t_hit = tcand;
	*point = vec_add(ray->origin, vec_mul(ray->direction, tcand));
	return (true);
}

//similar to cylinder
bool	intersect_cone(t_object *obj, t_ray *ray, double *t)
{
	t_cone	*cone;
	t_vec3	point;
	double	t_tmp;
	bool	hit;

	cone = (t_cone *)obj->shape;
	t_tmp = *t;
	hit = false;
	if (intersect_cone_caps(ray, cone, &t_tmp, &point))
	{
		*t = t_tmp;
		hit = true;
	}
	if (intersect_infinite_cone_side(ray, cone, &t_tmp, &point))
	{
		if (is_point_within_cone_height(point, cone) && t_tmp < *t)
		{
			*t = t_tmp;
			hit = true;
		}
	}
	return (hit);
}

//normal similar to cylinder, except shifted to account for cone angle
void	get_normal_cone(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal)
{
	t_cone	*cone;
	t_vec3	ap;
	double	proj;
	double	cos2;
	t_vec3	n;

	cone = (t_cone *)obj->shape;
	ap = vec_sub(*hit_point, cone->apex);
	proj = vec_dot(ap, cone->axis);
	if (proj >= cone->height - 1e-4)
	{
		*out_normal = cone->axis;
		return ;
	}
	if (proj <= 1e-6)
	{
		*out_normal = vec_mul(cone->axis, -1.0);
		return ;
	}
	cos2 = cos(cone->angle);
	cos2 = cos2 * cos2;
	n = vec_sub(vec_mul(cone->axis, proj), vec_mul(ap, cos2));
	*out_normal = vec_normalize(n);
}