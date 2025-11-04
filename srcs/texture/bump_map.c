/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//get height from gradient image at texel uv position on obj surface
//dhdu and dhdv averages of two height samples used to compute a 
//central difference
static void	bump_sample_gradients(t_bump_var *c)
{
	double	hx1;
	double	hx2;
	double	hy1;
	double	hy2;

	hx1 = sample_height01(c->img, c->uv.x + c->du, c->uv.y);
	hx2 = sample_height01(c->img, c->uv.x - c->du, c->uv.y);
	hy1 = sample_height01(c->img, c->uv.x, c->uv.y + c->dv);
	hy2 = sample_height01(c->img, c->uv.x, c->uv.y - c->dv);
	c->dhdu = 0.5 * (hx1 - hx2);
	c->dhdv = 0.5 * (hy1 - hy2);
}

//init vars
// if (c->img->width < 1 || c->img->height < 1)
// 	return (0);
//?
//redundant checks optimized by compiler better too safe than unsafe
static int	bump_init_vars(t_object *obj,
							t_vec3 hit_p, t_vec3 *normal, t_bump_var *c)
{
	t_material *mat = &obj->material;

	if (mat->texture.has_bump_maps != true)
		return (0);
	if (mat->texture.bumps_data == NULL)
		return (0);
	if (obj->get_uv == NULL)
		return (printf("3"), 0);
	if (obj->get_uv(obj, &hit_p, &c->uv) == false)
		return (printf("4"), 0);
	c->img = (t_texture_image *)mat->texture.bumps_data;
	c->obj = obj;
	c->mat = mat;
	c->hit_p = hit_p;
	c->du = 1.0 / (double)c->img->width;
	c->dv = 1.0 / (double)c->img->height;
	c->N = *normal;
	return (1);
}

//dispatch (cleanest approach i found for norm for norm 
//without having to resort to function pointer again, might rewrite that)
static void	bump_build_tangent(t_bump_var *c)
{
	if (c->obj->type == OBJ_PLANE)
		bump_tangent_plane(c);
	else if (c->obj->type == OBJ_TRIANGLE)
		bump_tangent_triangle(c);
	else if (c->obj->type == OBJ_SPHERE)
		bump_tangent_sphere(c);
	else if (c->obj->type == OBJ_CYLINDER)
		bump_tangent_cylinder(c);
	else if (c->obj->type == OBJ_CONE)
		bump_tangent_cone(c);
	else
	{
		ft_dprintf(2, "object type unrocognized ! %i", c->obj->type);
		build_tb_plane((t_plane *)c->obj->shape, c->N, &c->T, &c->B);
	}
}

//shift normal using T B and computed dhdu and dhdv values
static void	bump_apply_normal(t_bump_var *c, t_vec3 *normal)
{
	t_vec3	g;
	t_vec3	n2;

	g = vec_add(vec_mul(c->T, c->dhdu), vec_mul(c->B, c->dhdv));
	n2 = vec_add(c->N, vec_mul(g, c->mat->bump_strength));
	*normal = vec_safe_normalize(n2, c->N);
}

//applies a bump map on normal at hit point
void	apply_bump_from_image(t_object *obj, t_vec3 hit_p, t_vec3 *normal)
{
	t_bump_var	c;

	if (bump_init_vars(obj, hit_p, normal, &c) == 0)
		return ;
	bump_sample_gradients(&c);
	bump_build_tangent(&c);
	bump_apply_normal(&c, normal);
}
