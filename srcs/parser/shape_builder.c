/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 03:37:00 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/28 17:11:08 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	build_sphere(t_object *obj, char **line)
{
	t_sphere	*shape;

	obj->shape = malloc(sizeof(t_sphere));
	shape = obj->shape;
	if (!obj->shape)
		put_err("build_sphere : Failled to malloc t_sphere");
	*(t_sphere *)(obj->shape) = (t_sphere){0};
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));
	set_property(PROP_POSITION, &shape->center, *line++);
	set_property(PROP_SIZE, &shape->radius, *line++);
	set_property(PROP_COLOUR, &obj->material.base_color, *line++);
	parse_mats(&obj->material, line);
	obj->intersect = intersect_sphere;
	obj->get_normal = get_normal_sphere;
	obj->get_uv = get_uv_sphere;
}

void	build_plane(t_object *obj, char **line)
{
	t_plane	*shape;

	obj->shape = malloc(sizeof(t_plane));
	shape = obj->shape;
	if (!obj->shape)
		put_err("build_plane : Failled to malloc t_plane");
	*(t_plane *)(obj->shape) = (t_plane){0};
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));
	set_property(PROP_POSITION, &shape->point, *line++);
	set_property(PROP_DIRECTION, &shape->normal, *line++);
	set_property(PROP_COLOUR, &obj->material.base_color, *line++);
	parse_mats(&obj->material, line);
	obj->intersect = intersect_plane;
	obj->get_normal = get_normal_plane;
	obj->get_uv = get_uv_plane;
}

void	build_cylinder(t_object *obj, char **line)
{
	t_cylinder	*shape;

	shape = obj->shape;
	obj->shape = malloc(sizeof(t_cylinder));
	shape = obj->shape;
	if (!obj->shape)
		put_err("build_cylinder : Failled to malloc t_cylinder");
	*(t_cylinder *)(obj->shape) = (t_cylinder){0};
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));
	set_property(PROP_POSITION, &shape->center, *line++);
	set_property(PROP_DIRECTION, &shape->axis, *line++);
	shape->axis = vec_normalize(shape->axis);
	set_property(PROP_SIZE, &shape->radius, *line++);
	set_property(PROP_SIZE, &shape->height, *line++);
	set_property(PROP_COLOUR, &obj->material.base_color, *line++);
	parse_mats(&obj->material, line);
	obj->intersect = intersect_cylinder;
	obj->get_normal = get_normal_cylinder;
	obj->get_uv = get_uv_cylinder;
}

void	build_cone(t_object *obj, char **line)
{
	t_cone	*shape;

	obj->shape = malloc(sizeof(t_cone));
	shape = obj->shape;
	if (!obj->shape)
		put_err("build_cone : Failled to malloc t_cone");
	*(t_cone *)(obj->shape) = (t_cone){0};
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));
	set_property(PROP_POSITION, &shape->apex, *line++);
	set_property(PROP_DIRECTION, &shape->axis, *line++);
	shape->axis = vec_normalize(shape->axis);
	set_property(PROP_ANGLE, &shape->angle, *line++);
	set_property(PROP_SIZE, &shape->height, *line++);
	set_property(PROP_COLOUR, &obj->material.base_color, *line++);
	parse_mats(&obj->material, line);
	obj->intersect = intersect_cone;
	obj->get_normal = get_normal_cone;
	obj->get_uv = get_uv_cone;
}

void	build_triangle(t_object *obj, char	**line)
{
	t_triangle	*shape;

	obj->shape = malloc(sizeof(t_triangle));
	shape = obj->shape;
	if (!obj->shape)
		put_err("build_cone : Failled to malloc t_triangle");
	*(t_triangle *)(obj->shape) = (t_triangle){0};
	ft_lstadd_back(&g_scene(0)->objects, add_content(obj));
	set_property(PROP_POSITION, &shape->p0, *line++);
	set_property(PROP_POSITION, &shape->p1, *line++);
	set_property(PROP_POSITION, &shape->p2, *line++);
	set_property(PROP_COLOUR, &obj->material.base_color, *line++);
	set_property(PROP_UV, &shape->uv_mode, *line++);
	parse_mats(&obj->material, line);
	obj->intersect = intersect_triangle;
	obj->get_normal = get_normal_triangle;
	obj->get_uv = get_uv_triangle;
}
