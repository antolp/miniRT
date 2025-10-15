/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shape_builder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 03:37:00 by epinaud           #+#    #+#             */
/*   Updated: 2025/10/15 21:42:44 by epinaud          ###   ########.fr       */
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

t_object	*build_triangle(t_object *tri)
{
	t_object	*shape;

	shape = malloc(sizeof(t_triangle));
	return (shape);
}
