/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 03:37:00 by epinaud           #+#    #+#             */
/*   Updated: 2025/10/19 18:33:01 by epinaud          ###   ########.fr       */
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

t_object	*build_sphere(t_object *obj)
{
	printf(">>> Building sphere\n");
	obj->shape = malloc(sizeof(t_sphere));
	if (!obj->shape)
		put_err("build_sphere : Failled to malloc t_sphere");
	obj->intersect = intersect_sphere;
	obj->get_normal = get_normal_sphere;
	obj->get_uv = get_uv_sphere;
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));
	return (obj);
}

t_object	*build_plane(t_object *obj)
{
	printf(">>> Building plane\n");
	obj->shape = malloc(sizeof(t_plane));
	if (!obj->shape)
		put_err("build_plane : Failled to malloc t_plane");
	obj->intersect = intersect_plane;
	obj->get_normal = get_normal_plane;
	obj->get_uv = get_uv_plane;
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));
	return (obj);
}

t_object	*build_cylinder(t_object *obj)
{
	printf(">>> Building cylinder\n");
	obj->shape = malloc(sizeof(t_cylinder));
	if (!obj->shape)
		put_err("build_cylinder : Failled to malloc t_cylinder");
	obj->intersect = intersect_cylinder;
	obj->get_normal = get_normal_cylinder;
	obj->get_uv = get_uv_cylinder;
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));
	return (obj);
}

t_object	*build_cone(t_object *obj)
{
	printf(">>> Building cone\n");
	obj->shape = malloc(sizeof(t_cone));
	if (!obj->shape)
		put_err("build_cone : Failled to malloc t_cone");
	obj->intersect = intersect_cone;
	obj->get_normal = get_normal_cone;
	obj->get_uv = get_uv_cone;
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));
	return (obj);
}

t_object	*build_triangle(t_object *obj, char	**line)
{
	t_triangle	*shape;

	printf(">>> Building triangle\n");
	shape = malloc(sizeof(t_triangle));
	if (!shape)
		put_err("build_triangle : Failled to malloc t_triangle");
	obj->shape = shape;
	obj->intersect = intersect_triangle;
	obj->get_normal = get_normal_triangle;
	obj->get_uv = get_uv_triangle;
	set_property(PROP_POSITION, (&((t_triangle *)obj->shape)->p0), line++);
	set_property(PROP_POSITION, (&((t_triangle *)obj->shape)->p1), line++);
	set_property(PROP_POSITION, (&((t_triangle *)obj->shape)->p2), line++);
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));

	printf("Triangle coordinates : %f,%f,%f, %f,%f,%f etc\n", shape->p0.x, shape->p0.y, shape->p0.z, shape->p1.x, shape->p1.y, shape->p1.z);
	return (obj);
}
