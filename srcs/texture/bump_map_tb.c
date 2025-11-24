/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map_tb.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//per-shape tangents bitangents
//N must be the already flipped shading normal at the hit
//T tangent, B bitangent. dot product of any vector between N, T and B is 0
//(all perpendicular to each other)
//
//
//for plane, choose a relative "up" not parallel to N (Y as up, fallback to X as up).
//rest is very straightforward, T = up × N  and  B = T * N
void	build_tb_plane(t_plane *pl, t_vec3 N, t_vec3 *T, t_vec3 *B)
{
	t_vec3 up;
	double d;

	(void)pl;
	up = (t_vec3){0.0, 1.0, 0.0};
	d = fabs(vec_dot(N, up));
	if (d > 0.999)
		up = (t_vec3){1.0, 0.0, 0.0};
	*T = vec_safe_normalize(vec_cross(up, N), (t_vec3){1.0, 0.0, 0.0});
	*B = vec_cross(N, *T);
}

//T and B aligned to an edge and the normal.
//T = p1 − p0 (fallback if too short)  and  B = N × T
void	build_tb_triangle_ortho(t_triangle *tr, t_vec3 N, t_vec3 *T, t_vec3 *B)
{
	t_vec3 Ue;
	t_vec3 Ve;
	double l;

	Ue = vec_sub(tr->p1, tr->p0);
	l = vec_length(Ue);
	if (l <= 1e-12)
		Ue = (t_vec3){1.0, 0.0, 0.0};
	else
		Ue = vec_mul(Ue, 1.0 / l);
	Ve = vec_cross(N, Ue);
	*T = Ue;
	*B = vec_safe_normalize(Ve, (t_vec3){0.0, 1.0, 0.0});
}

//This time, T and B from two edges using Gram–Schmidt
//(read how TRI_UV_FIT uv modes are implemented in the uv_triangle.c file may help)
//T = e0 = p1 − p0.							(on u axis)
//B = e1 = (p2 − p0) − T * dot(p2 − p0, T)	(on v-axis)
//(for B, the math in the function are equivalent to what's written here)
//so B perpendicular T and both in the triangle
//this fixes a bug where two triangle with matching UV_FIT and UV_FIT_OPPOSITE modes
//wouldn't have matching bump map orientation along shared egde.
void	build_tb_triangle_fit(t_triangle *tr, t_vec3 N, t_vec3 *T, t_vec3 *B)
{
	t_vec3 e0;
	t_vec3 e1;
	double l;

	e0 = vec_sub(tr->p1, tr->p0);
	l = vec_length(e0);
	if (l <= 1e-12)
		e0 = (t_vec3){1.0, 0.0, 0.0};
	else
		e0 = vec_mul(e0, 1.0 / l);
	e1 = vec_sub(tr->p2, tr->p0);
	e1 = vec_sub(e1, vec_mul(e0, vec_dot(e1, e0)));
	*T = e0;
	*B = vec_safe_normalize(e1, (t_vec3){0.0, 1.0, 0.0});
	(void)N;
}

//T ~= normalize((−Nz, 0, Nx))
//if N is at a pole, fallback to X axis 
//if you draw it on a piece of paper, you can see it's correct (pretty cool)
//B = N * T as always
//
//https://computergraphics.stackexchange.com/questions/
//5498/compute-sphere-tangent-for-normal-mapping
//here, we arent using spherical coordinates, but that little trick works
//using 3d vector coordinates
void	build_tb_sphere(t_sphere *sp, t_vec3 N, t_vec3 *T, t_vec3 *B)
{
	t_vec3	fallback;
	t_vec3	t;

	t = (t_vec3){-N.z, 0.0, N.x};
	if (fabs(N.y) > 0.999)
		fallback = (t_vec3){1.0, 0.0, 0.0};
	else
		fallback = t;
	*T = vec_safe_normalize(t, fallback);
	*B = vec_cross(N, *T);
	(void)sp;
}
