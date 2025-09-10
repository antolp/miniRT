/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//equivalent to fmin(fmax(x, 0.0), 1.0);
//easier to read
//will probably replace previous expressions with calls to this
double	clamp01(double x)
{
	if (x < 0.0)
		return (0.0);
	if (x > 1.0)
		return (1.0);
	return (x);
}

//equivalent to x - floor(x);
double	wrap01(double x)
{
	while (x >= 1.0)
		x = x - 1.0;
	while (x < 0.0)
		x = x + 1.0;
	return (x);
}

//checkeroard_uv():
//Computes the checkerboard color at a given UV coordinate.
//alternates between two colors based on the sum of integer UV grid coords
//nice use of bitwise operator to check if even or odd
//useful to debug UV unwrap
t_color  checkeroard_uv(const t_checkerboard *cb, t_vec2 uv)
{
    t_color out;
    int     iu;
    int     iv;
    int     parity;

    if (cb->scale_u <= 0.0 || cb->scale_v <= 0.0)
        return (cb->color1);
    iu = (int)floor(uv.x * cb->scale_u);
    iv = (int)floor(uv.y * cb->scale_v);
    parity = (iu + iv) & 1;
    out = cb->color1;
    if (parity != 0)
        out = cb->color2;
    return (out);
}

//for norm and readability
static bool	object_uv(t_object *obj, t_vec3 *hit_p, t_vec2 *uv)
{
	if (obj == NULL || obj->get_uv == NULL)
		return (false);
	return (obj->get_uv(obj, hit_p, uv));
}

//returns the color at this hit point:
//	- if CHECKER flag on and texture is TEX_CHECKER -> procedural color
//	- otherwise -> material.base_color (image sampling to be added later)
t_color	get_hit_color(t_hit_info *hit)
{
	t_object		*obj;
	t_material		*mat;
	unsigned int	flags;
	t_vec2			uv;
	t_texture		*tex;
	t_checkerboard	*chk;

	obj = hit->object;
	mat = &obj->material;
	flags = g_renderer(NULL)->shading_flag;

	if ((flags & TEXTURE_CHECKER) != 0u)
	{
		if (mat->texture.type == TEXTURE_CHECKER && mat->texture.data != NULL)
		{
			if (object_uv(obj, &hit->hit_point, &uv) == true)
			{
				/* planes produce unbounded uv; wrap to [0,1) */
				tex = &mat->texture;
				chk = (t_checkerboard *)tex->data;
				return (checkeroard_uv(chk, uv));
			}
		}
	}
	return (mat->base_color);
}
