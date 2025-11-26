/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_obj_pitch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:29:59 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:30:00 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	editor_rotate_plane_pitch(t_object *obj, t_vec3 axis,
			double angle_deg)
{
	t_plane	*pl;

	pl = (t_plane *)obj->shape;
	pl->normal = vec_rotate_axis(pl->normal, axis, angle_deg);
}

static void	editor_rotate_cylinder_pitch(t_object *obj, t_vec3 axis,
			double angle_deg)
{
	t_cylinder	*cyl;

	cyl = (t_cylinder *)obj->shape;
	cyl->axis = vec_rotate_axis(cyl->axis, axis, angle_deg);
}

static void	editor_rotate_cone_pitch(t_object *obj, t_vec3 axis,
			double angle_deg)
{
	t_cone	*cone;

	cone = (t_cone *)obj->shape;
	cone->axis = vec_rotate_axis(cone->axis, axis, angle_deg);
}

void	editor_rotate_triangle_pitch(t_object *obj, t_vec3 axis,
			double angle_deg)
{
	t_triangle	*tri;
	t_vec3		center;
	t_vec3		off;

	tri = (t_triangle *)obj->shape;
	center = editor_triangle_center(tri);
	off = vec_sub(tri->p0, center);
	off = vec_rotate_axis(off, axis, angle_deg);
	tri->p0 = vec_add(center, off);
	off = vec_sub(tri->p1, center);
	off = vec_rotate_axis(off, axis, angle_deg);
	tri->p1 = vec_add(center, off);
	off = vec_sub(tri->p2, center);
	off = vec_rotate_axis(off, axis, angle_deg);
	tri->p2 = vec_add(center, off);
}

//object pitch main entry
void	editor_rotate_object_pitch(t_cli_var *var, double angle_deg)
{
	t_object	*obj;

	if (var->r->edit.current_obj == NULL)
		return ;
	obj = (t_object *)var->r->edit.current_obj->content;
	if (obj->type == OBJ_PLANE)
		editor_rotate_plane_pitch(obj, var->right, angle_deg);
	else if (obj->type == OBJ_CYLINDER)
		editor_rotate_cylinder_pitch(obj, var->right, angle_deg);
	else if (obj->type == OBJ_CONE)
		editor_rotate_cone_pitch(obj, var->right, angle_deg);
	else if (obj->type == OBJ_TRIANGLE)
		editor_rotate_triangle_pitch(obj, var->right, angle_deg);
	var->r->has_drawn_realtime = false;
}
