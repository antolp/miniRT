/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scale.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:30:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/27 16:45:26 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	editor_scale_triangle_uniform(t_object *obj, double factor)
{
	t_triangle	*tri;
	t_vec3		center;
	t_vec3		off;

	tri = (t_triangle *)obj->shape;
	center = editor_triangle_center(tri);
	off = vec_sub(tri->p0, center);
	off = vec_mul(off, factor);
	tri->p0 = vec_add(center, off);
	off = vec_sub(tri->p1, center);
	off = vec_mul(off, factor);
	tri->p1 = vec_add(center, off);
	off = vec_sub(tri->p2, center);
	off = vec_mul(off, factor);
	tri->p2 = vec_add(center, off);
}

void	editor_scale_object_primary(t_cli_var *var, double factor)
{
	t_object	*obj;

	if (var->r->edit.current_obj == NULL)
		return ;
	obj = (t_object *)var->r->edit.current_obj->content;
	if (obj->type == OBJ_SPHERE)
		editor_scale_sphere_radius(obj, factor);
	else if (obj->type == OBJ_CYLINDER)
		editor_scale_cylinder_radius(obj, factor);
	else if (obj->type == OBJ_CONE)
		editor_scale_cone_height(obj, factor);
	else if (obj->type == OBJ_TRIANGLE)
		editor_scale_triangle_uniform(obj, factor);
	var->r->has_drawn_realtime = false;
}

void	editor_scale_object_secondary(t_cli_var *var, double factor)
{
	t_object	*obj;

	if (var->r->edit.current_obj == NULL)
		return ;
	obj = (t_object *)var->r->edit.current_obj->content;
	if (obj->type == OBJ_CYLINDER)
		editor_scale_cylinder_height(obj, factor);
	else if (obj->type == OBJ_CONE)
	{
		editor_scale_cone_angle(obj, factor);
		printf("factor : %f\n", factor);
	}
	var->r->has_drawn_realtime = false;
}
