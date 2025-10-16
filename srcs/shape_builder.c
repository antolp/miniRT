/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 03:37:00 by epinaud           #+#    #+#             */
/*   Updated: 2025/10/16 05:38:32 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

/* 		[OBJ_AMBIANT_LIGHT]
		[OBJ_CAMERA]
		[OBJ_LIGHT]
		[OBJ_BACKGROUND
		[OBJ_SKYBOX]
		[OBJ_SPHERE]
		[OBJ_PLANE] 
		[OBJ_CYLINDER] 
		[OBJ_CONE]
		[OBJ_TRIANGLE] 
 */

t_object	*build_sphere()
{
	t_object	*shape;

	shape = malloc(sizeof(t_sphere));
	return (shape);
}

t_object	*build_plane()
{
	t_object	*shape;

	shape = malloc(sizeof(t_plane));
	return (shape);
}

t_object	*build_cylinder()
{
	t_object	*shape;

	shape = malloc(sizeof(t_cylinder));
	return (shape);
}

t_object	*build_cone()
{
	t_object	*shape;

	shape = malloc(sizeof(t_cone));
	return (shape);
}

t_object	*build_triangle(t_object *obj)
{
	t_triangle	*shape;
	t_list		*objhead;

	printf(">>> Building triangle\n");
	shape = malloc(sizeof(t_triangle));
	if (!shape)
		put_err("build_triangle : Failled to malloc t_triangle");
	obj->shape = shape;
	obj->intersect = intersect_triangle;
	obj->get_normal = get_normal_triangle;
	obj->get_uv = get_uv_triangle;
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));
	return (obj);
}
