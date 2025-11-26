/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:30:05 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:49:11 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

double	deg2rad(double angle)
{
	return (angle * M_PI / 180.0);
}

//Rotates around Y axis
t_vec3	vec_rotate_yaw(t_vec3 v, double angle_deg)
{
	double	angle;
	double	cos_a;
	double	sin_a;
	t_vec3	r;

	angle = deg2rad(angle_deg);
	cos_a = cos(angle);
	sin_a = sin(angle);
	r.x = v.x * cos_a + v.z * sin_a;
	r.y = v.y;
	r.z = -v.x * sin_a + v.z * cos_a;
	return (r);
}

t_vec3	rotate_yaw(t_vec3 dir, double angle_deg)
{
	t_vec3	r;

	r = vec_rotate_yaw(dir, angle_deg);
	return (vec_normalize(r));
}

//rotates around right vector using Rodrigues' rotation formula
static t_vec3	rotate_pitch_rodrigues(t_vec3 dir, t_vec3 right,
			double cos_a, double sin_a)
{
	double	dot_rd;
	t_vec3	cross_rd;
	t_vec3	new_dir;

	dot_rd = vec_dot(right, dir);
	cross_rd = vec_cross(right, dir);
	new_dir.x = dir.x * cos_a + right.x * dot_rd * (1.0 - cos_a)
		+ cross_rd.x * sin_a;
	new_dir.y = dir.y * cos_a + right.y * dot_rd * (1.0 - cos_a)
		+ cross_rd.y * sin_a;
	new_dir.z = dir.z * cos_a + right.z * dot_rd * (1.0 - cos_a)
		+ cross_rd.z * sin_a;
	return (vec_normalize(new_dir));
}

t_vec3	rotate_pitch(t_vec3 dir, double angle_deg)
{
	double	angle;
	double	cos_a;
	double	sin_a;
	t_vec3	world_up;
	t_vec3	right;

	angle = deg2rad(angle_deg);
	cos_a = cos(angle);
	sin_a = sin(angle);
	world_up = (t_vec3){0.0, 1.0, 0.0};
	right = vec_normalize(vec_cross(dir, world_up));
	return (rotate_pitch_rodrigues(dir, right, cos_a, sin_a));
}
