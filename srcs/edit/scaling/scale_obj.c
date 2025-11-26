/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scale_obj.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:30:10 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:30:10 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	editor_scale_sphere_radius(t_object *obj, double factor)
{
	t_sphere	*s;

	s = (t_sphere *)obj->shape;
	s->radius = s->radius * factor;
}

void	editor_scale_cylinder_radius(t_object *obj, double factor)
{
	t_cylinder	*cyl;

	cyl = (t_cylinder *)obj->shape;
	cyl->radius = cyl->radius * factor;
}

void	editor_scale_cylinder_height(t_object *obj, double factor)
{
	t_cylinder	*cyl;

	cyl = (t_cylinder *)obj->shape;
	cyl->height = cyl->height * factor;
}

void	editor_scale_cone_height(t_object *obj, double factor)
{
	t_cone	*cone;

	cone = (t_cone *)obj->shape;
	cone->height = cone->height * factor;
}

void	editor_scale_cone_angle(t_object *obj, double factor)
{
	t_cone	*cone;

	cone = (t_cone *)obj->shape;
	cone->angle = cone->angle * factor;
	if (cone->angle < 1.0)
		cone->angle = 1.0;
	if (cone->angle > 89.0)
		cone->angle = 89.0;
}
