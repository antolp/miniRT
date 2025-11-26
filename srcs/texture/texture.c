/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 05:56:41 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//checkeroard_uv():
//Computes the checkerboard color at a given UV coordinate.
//alternates between two colors based on the sum of integer UV grid coords
//nice use of bitwise operator to check if even or odd
//useful to debug UV unwrap
t_color	checkeroard_uv(const t_checkerboard *cb, t_vec2 uv)
{
	t_color	out;
	int		iu;
	int		iv;
	int		parity;

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

//to add readability, avoid redundances
static bool	object_uv(t_object *obj, t_vec3 *hit_p, t_vec2 *uv)
{
	if (obj == NULL || obj->get_uv == NULL)
		return (false);
	return (obj->get_uv(obj, hit_p, uv));
}

bool	get_checker_color(t_object *obj, t_material *mat,
			t_hit_info *hit, t_color *out)
{
	t_vec2			uv;
	t_texture		*tex;
	t_checkerboard	*chk;

	if (mat->texture.type != TEXTURE_CHECKER)
		return (false);
	if (mat->texture.data == NULL)
		return (false);
	if (object_uv(obj, &hit->hit_point, &uv) == false)
		return (false);
	tex = &mat->texture;
	chk = (t_checkerboard *)tex->data;
	*out = checkeroard_uv(chk, uv);
	return (true);
}

bool	get_image_color(t_object *obj, t_material *mat,
			t_hit_info *hit, t_color *out)
{
	t_vec2		uv;
	t_texture	*tex;
	t_tex_img	*img;

	if (mat->texture.type != TEXTURE_IMAGE)
		return (false);
	if (mat->texture.data == NULL)
		return (false);
	if (object_uv(obj, &hit->hit_point, &uv) == false)
		return (false);
	tex = &mat->texture;
	img = (t_tex_img *)tex->data;
	uv.x = wrap01(uv.x);
	uv.y = wrap01(uv.y);
	*out = sample_image_nearest(img, uv.x, uv.y);
	return (true);
}

//returns the color at given hit point
//either checker, image sampling or base_color
t_color	get_hit_color(t_hit_info *hit)
{
	t_object		*obj;
	t_material		*mat;
	unsigned int	flags;
	t_color			color;

	obj = hit->object;
	mat = &obj->material;
	flags = g_renderer(NULL)->shading_flag;
	if ((flags & FLAG_TEXTURE) != 0u)
	{
		if (get_checker_color(obj, mat, hit, &color) == true)
			return (color);
		if (get_image_color(obj, mat, hit, &color) == true)
			return (color);
	}
	return (mat->base_color);
}
