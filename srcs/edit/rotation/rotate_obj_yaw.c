/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate_obj_yaw.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:30:02 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:51:46 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//for rotations, since we can rotate only four object
//theyre all static helpers
static void	editor_rotate_plane_yaw(t_object *obj, double angle_deg)
{
	t_plane	*pl;

	pl = (t_plane *)obj->shape;
	pl->normal = rotate_yaw(pl->normal, angle_deg);
}

static void	editor_rotate_cylinder_yaw(t_object *obj, double angle_deg)
{
	t_cylinder	*cyl;

	cyl = (t_cylinder *)obj->shape;
	cyl->axis = rotate_yaw(cyl->axis, angle_deg);
}

static void	editor_rotate_cone_yaw(t_object *obj, double angle_deg)
{
	t_cone	*cone;

	cone = (t_cone *)obj->shape;
	cone->axis = rotate_yaw(cone->axis, angle_deg);
}

//may not explain this one
static void	editor_rotate_triangle_yaw(t_object *obj, double angle_deg)
{
	t_triangle	*tri;
	t_vec3		center;
	t_vec3		off;

	tri = (t_triangle *)obj->shape;
	center = editor_triangle_center(tri);
	off = vec_sub(tri->p0, center);
	off = vec_rotate_yaw(off, angle_deg);
	tri->p0 = vec_add(center, off);
	off = vec_sub(tri->p1, center);
	off = vec_rotate_yaw(off, angle_deg);
	tri->p1 = vec_add(center, off);
	off = vec_sub(tri->p2, center);
	off = vec_rotate_yaw(off, angle_deg);
	tri->p2 = vec_add(center, off);
}

//object yaw main entry
void	editor_rotate_object_yaw(t_cli_var *var, double angle_deg)
{
	t_object	*obj;

	if (var->r->edit.current_obj == NULL)
		return ;
	obj = (t_object *)var->r->edit.current_obj->content;
	if (obj->type == OBJ_PLANE)
		editor_rotate_plane_yaw(obj, angle_deg);
	else if (obj->type == OBJ_CYLINDER)
		editor_rotate_cylinder_yaw(obj, angle_deg);
	else if (obj->type == OBJ_CONE)
		editor_rotate_cone_yaw(obj, angle_deg);
	else if (obj->type == OBJ_TRIANGLE)
		editor_rotate_triangle_yaw(obj, angle_deg);
	var->r->has_drawn_realtime = false;
}
