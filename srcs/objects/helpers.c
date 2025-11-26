/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 03:03:20 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 03:17:15 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//i == 0 gets base center
//else gets top cap center
//writing normal here is useless
void	get_cap_info(t_cylinder *cyl, int i, t_vec3 *center, t_vec3 *normal)
{
	t_vec3	axis_n;

	axis_n = vec_normalize(cyl->axis);
	if (i == 0)
	{
		*center = cyl->center;
		*normal = vec_mul(axis_n, -1.0);
	}
	else
	{
		*center = vec_add(cyl->center, vec_mul(axis_n, cyl->height));
		*normal = axis_n;
	}
}

//hit must lie between apex (0) and base (height)
//use dot product to compare vector projection
bool	is_point_within_cylinder_height(t_vec3 point, t_cylinder *cyl)
{
	t_vec3	v;
	double	proj;

	v = vec_sub(point, cyl->center);
	proj = vec_dot(v, cyl->axis);
	return (proj >= 0.0 && proj <= cyl->height);
}

//helper for norm
void	compute_quad_cyl(t_quad *q, t_cyl_side_vars v, double radius)
{
	q->a = vec_dot(v.d_perp, v.d_perp);
	q->b = 2.0 * vec_dot(v.d_perp, v.cto_perp);
	q->c = vec_dot(v.cto_perp, v.cto_perp) - radius * radius;
	q->d = q->b * q->b - 4.0 * q->a * q->c;
}

//helper for norm
void	compute_quad_cone(t_quad *q, t_cone_side_vars v)
{
	q->a = v.dv * v.dv - v.cos2 * vec_dot(v.d, v.d);
	q->b = 2.0 * (v.dv * v.cov - v.cos2 * vec_dot(v.d, v.co));
	q->c = v.cov * v.cov - v.cos2 * vec_dot(v.co, v.co);
	q->d = q->b * q->b - 4.0 * q->a * q->c;
	return ;
}
