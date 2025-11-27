/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   props_rotate.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:29:57 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/27 01:49:23 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//split for norm
t_vec3	vec_rotate_axis_helper(t_vec3 v, t_vec3 a,
			double cos_a, double sin_a)
{
	double	dot_av;
	t_vec3	cross_av;
	t_vec3	res;

	dot_av = vec_dot(a, v);
	cross_av = vec_cross(a, v);
	res.x = v.x * cos_a + a.x * dot_av * (1.0 - cos_a)
		+ cross_av.x * sin_a;
	res.y = v.y * cos_a + a.y * dot_av * (1.0 - cos_a)
		+ cross_av.y * sin_a;
	res.z = v.z * cos_a + a.z * dot_av * (1.0 - cos_a)
		+ cross_av.z * sin_a;
	return (res);
}

//rotate axis to given angle
t_vec3	vec_rotate_axis(t_vec3 v, t_vec3 axis, double angle_deg)
{
	double	angle;
	double	cos_a;
	double	sin_a;
	t_vec3	a;

	angle = deg2rad(angle_deg);
	cos_a = cos(angle);
	sin_a = sin(angle);
	a = vec_normalize(axis);
	return (vec_rotate_axis_helper(v, a, cos_a, sin_a));
}

int	handle_props_rotate_yaw(int keycode, t_cli_var *var)
{
	double	angle;

	if (keycode != XK_k && keycode != XK_K
		&& keycode != XK_semicolon)
		return (0);
	if (var->r->edit.target == EDIT_TARGET_LIGHT)
	{
		printf("Lights cannot be rotated.\n");
		return (1);
	}
	if (var->r->edit.target != EDIT_TARGET_OBJECT)
		return (1);
	angle = var->scale * 7;
	if (keycode == XK_k || keycode == XK_K)
		angle = -var->scale * 7;
	editor_rotate_object_yaw(var, angle);
	return (1);
}

int	handle_props_rotate_pitch(int keycode, t_cli_var *var)
{
	double	angle;

	if (keycode != XK_o && keycode != XK_O
		&& keycode != XK_l && keycode != XK_L)
		return (0);
	if (var->r->edit.target == EDIT_TARGET_LIGHT)
	{
		printf("Lights cannot be rotated.\n");
		return (1);
	}
	if (var->r->edit.target != EDIT_TARGET_OBJECT)
		return (1);
	angle = var->scale * 7;
	if (keycode == XK_o || keycode == XK_O)
		angle = var->scale * 7;
	else
		angle = -var->scale * 7;
	editor_rotate_object_pitch(var, angle);
	return (1);
}

//helper for triangle rotation
t_vec3	editor_triangle_center(t_triangle *tri)
{
	t_vec3	center;

	center = vec_add(tri->p0, tri->p1);
	center = vec_add(center, tri->p2);
	center = vec_mul(center, 1.0 / 3.0);
	return (center);
}
