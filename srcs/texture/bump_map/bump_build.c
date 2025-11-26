/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_build.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 05:27:03 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//per-type tangent builders (norm-unduced redundance hell)
void	bump_tangent_plane(t_bump_var *c)
{
	build_tb_plane((t_plane *)c->obj->shape, c->normal,
		&c->tangent, &c->bitangent);
}

void	bump_tangent_triangle(t_bump_var *c)
{
	t_triangle	*tr;

	tr = (t_triangle *)c->obj->shape;
	if (tr->uv_mode == TRI_UV_FIT || tr->uv_mode == TRI_UV_FIT_OPPOSITE)
		build_tb_triangle_fit(tr, &c->tangent, &c->bitangent);
	else
		build_tb_triangle_ortho(tr, c->normal, &c->tangent, &c->bitangent);
}

void	bump_tangent_cylinder(t_bump_var *c)
{
	if (is_cylinder_cap((t_cylinder *)c->obj->shape, c->hit_p))
		build_tb_plane(NULL, c->normal, &c->tangent, &c->bitangent);
	else
		build_tb_cylinder_side((t_cylinder *)c->obj->shape, c->hit_p,
			&c->tangent, &c->bitangent);
}

void	bump_tangent_cone(t_bump_var *c)
{
	if (is_cone_base((t_cone *)c->obj->shape, c->hit_p))
		build_tb_plane(NULL, c->normal, &c->tangent, &c->bitangent);
	else
		build_tb_cone_side((t_cone *)c->obj->shape, c->normal,
			&c->tangent, &c->bitangent);
}

void	bump_tangent_sphere(t_bump_var *c)
{
	build_tb_sphere((t_sphere *)c->obj->shape, c->normal,
		&c->tangent, &c->bitangent);
}
