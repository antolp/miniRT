/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   translate_obj.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:30:17 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/27 01:16:29 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//object specific translation functions
void	editor_translate_sphere(t_sphere *s, t_vec3 delta)
{
	s->center = vec_add(s->center, delta);
}

void	editor_translate_plane(t_plane *p, t_vec3 delta)
{
	p->point = vec_add(p->point, delta);
}

void	editor_translate_cylinder(t_cylinder *c, t_vec3 delta)
{
	c->center = vec_add(c->center, delta);
}

void	editor_translate_cone(t_cone *c, t_vec3 delta)
{
	c->apex = vec_add(c->apex, delta);
}

void	editor_translate_triangle(t_triangle *t, t_vec3 delta)
{
	t->p0 = vec_add(t->p0, delta);
	t->p1 = vec_add(t->p1, delta);
	t->p2 = vec_add(t->p2, delta);
}
