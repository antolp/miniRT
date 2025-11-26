/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 04:47:35 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//two next function reuse the hit_intersection logic
//it's redundant, may fix this later
//
int	is_cylinder_cap( t_cylinder *cy, t_vec3 p)
{
	t_vec3	a;
	double	h;
	double	eps;

	a = vec_safe_normalize(cy->axis, (t_vec3){0.0, 1.0, 0.0});
	h = vec_dot(vec_sub(p, cy->center), a);
	eps = fmax(1e-6, 5e-4 * cy->height);
	if (fabs(h) <= eps)
		return (1);
	if (fabs(h - cy->height) <= eps)
		return (1);
	return (0);
}

int	is_cone_base( t_cone *co, t_vec3 p)
{
	t_vec3	a;
	t_vec3	c_b;
	double	d;

	a = vec_safe_normalize(co->axis, (t_vec3){0.0, 1.0, 0.0});
	c_b = vec_add(co->apex, vec_mul(a, co->height));
	d = fabs(vec_dot(vec_sub(p, c_b), a));
	if (d <= fmax(1e-6, 5e-4 * co->height))
		return (1);
	return (0);
}

//Cb bottom center
//T direction from bottom center, fallback right direction
//(shouldn't happen but it happens somehow)
//B from cylinder axis
void	build_tb_cylinder_side( t_cylinder *cy, t_vec3 p, t_vec3 *t, t_vec3 *b)
{
	t_vec3	a;
	t_vec3	rdir;
	t_vec3	c_b;
	t_vec3	d;
	double	h;

	a = vec_safe_normalize(cy->axis, (t_vec3){0.0, 1.0, 0.0});
	c_b = cy->center;
	d = vec_sub(p, c_b);
	h = vec_dot(d, a);
	rdir = vec_sub(d, vec_mul(a, h));
	rdir = vec_safe_normalize(rdir, (t_vec3){1.0, 0.0, 0.0});
	*t = vec_safe_normalize(vec_cross(a, rdir), (t_vec3){0.0, 0.0, 1.0});
	*b = a;
}

//T direction around cone
//B from slant
//should do a drawing but reading the values doesnt make it too 
//hard to get a rough idea, idk
void	build_tb_cone_side( t_cone *co, t_vec3 n, t_vec3 *t, t_vec3 *b)
{
	t_vec3	a;

	a = vec_safe_normalize(co->axis, (t_vec3){0.0, 1.0, 0.0});
	*t = vec_safe_normalize(vec_cross(a, n), (t_vec3){1.0, 0.0, 0.0});
	*b = vec_safe_normalize(vec_sub(a, vec_mul(n, vec_dot(a, n))),
			(t_vec3){0.0, 1.0, 0.0});
}
