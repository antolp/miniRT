/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//magic numbers convert luminance (perceived brightness) into height scalars
//google "Rec.709 sRGB luminance coefficients", that's how you're supposed to do it
static double	color_to_height01(t_color c)
{
	return ((0.2126 * (double)c.r + 0.7152 * (double)c.g + 0.0722 * (double)c.b) / 255.0);
}

//uv than return height at point
double	sample_height01(const t_texture_image *img, double u, double v)
{
	t_color px;

	u = wrap01(u);
	v = wrap01(v);
	px = sample_image_nearest(img, u, v);
	return (color_to_height01(px));
}

//two next function reuse the hit_intersection logic
//it's redundant, may fix this later
//
int	is_cylinder_cap( t_cylinder *cy, t_vec3 P)
{
	t_vec3 A;
	double h;
	double eps;

	A = vec_safe_normalize(cy->axis, (t_vec3){0.0, 1.0, 0.0});
	h = vec_dot(vec_sub(P, cy->center), A);
	eps = fmax(1e-6, 5e-4 * cy->height);
	if (fabs(h) <= eps)
		return (1);
	if (fabs(h - cy->height) <= eps)
		return (1);
	return (0);
}

int	is_cone_base( t_cone *co, t_vec3 P)
{
	t_vec3 A;
	t_vec3 Cb;
	double d;

	A = vec_safe_normalize(co->axis, (t_vec3){0.0, 1.0, 0.0});
	Cb = vec_add(co->apex, vec_mul(A, co->height));
	d = fabs(vec_dot(vec_sub(P, Cb), A));
	if (d <= fmax(1e-6, 5e-4 * co->height))
		return (1);
	return (0);
}

//Cb bottom center
//T direction from bottom center, fallback right direction
//(shouldn't happen but it happens somehow)
//B from cylinder axis
void	build_tb_cylinder_side( t_cylinder *cy, t_vec3 P, t_vec3 N, t_vec3 *T, t_vec3 *B)
{
	t_vec3 A;
	t_vec3 rdir;
	t_vec3 Cb;
	t_vec3 d;
	double h;

	A = vec_safe_normalize(cy->axis, (t_vec3){0.0, 1.0, 0.0});
	Cb = cy->center;
	d = vec_sub(P, Cb);
	h = vec_dot(d, A);
	rdir = vec_sub(d, vec_mul(A, h));
	rdir = vec_safe_normalize(rdir, (t_vec3){1.0, 0.0, 0.0});
	(void)N;
	*T = vec_safe_normalize(vec_cross(A, rdir), (t_vec3){0.0, 0.0, 1.0});
	*B = A;
}

//T direction around cone
//B from slant
//should do a drawing but reading the values doesnt make it too 
//hard to get a rough idea, idk
void	build_tb_cone_side( t_cone *co, t_vec3 P, t_vec3 N, t_vec3 *T, t_vec3 *B)
{
	t_vec3 A;

	A = vec_safe_normalize(co->axis, (t_vec3){0.0, 1.0, 0.0});
	*T = vec_safe_normalize(vec_cross(A, N), (t_vec3){1.0, 0.0, 0.0});
	*B = vec_safe_normalize(vec_sub(A, vec_mul(N, vec_dot(A, N))), 
		(t_vec3){0.0, 1.0, 0.0});
	(void)P;
}