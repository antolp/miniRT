/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_sample.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 18:29:53 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	clamp(int v, int lo, int hi)
{
	if (v < lo)
		return (lo);
	if (v > hi)
		return (hi);
	return (v);
}

//reads one texel at byte offset 'byte_index' and returns it as t_color
//always 32bpp BGRA in memory (img->bgra != 0), cham
//for norm
static t_color	fetch_texel(const t_tex_img *img, int byte_index)
{
	unsigned char	*p;
	t_color			c;

	p = (unsigned char *)(img->addr + byte_index);
	if (img->bgra != 0)
	{
		c.r = (int)p[2];
		c.g = (int)p[1];
		c.b = (int)p[0];
	}
	else
	{
		c.r = (int)p[0];
		c.g = (int)p[1];
		c.b = (int)p[2];
	}
	return (c);
}

//sample_image_nearest():
//Nearest-neighbor texel fetch from an MLX image, 
//
//Wrap UVs into [0,1] (tile repeat) with wrap01(); flip V (vf = 1-vf)
//	because MLX/X11 image origin is top-left while UV space is bottom-left
//Convert UVs to nearest integer pixel coords:
//	x = floor( u * (width  - 1) )
//	y = floor( v * (height - 1) )
//Then clamp with clampi() so edge UVs don’t step outside
//Compute byte index into the image buffer : fetch_texel()
//	idx = y * line_len + x * (bpp/8)
//	where line_len is the stride in BYTES per scanline, and bpp is bits/pixel
//	(trial and error, not sure exactly )
//Read 3 color bytes from the pixel pointer.
//mlx stores 32-bpp in BGRA byte order (img->bgra != 0), so channels are:
//		BGRA: ptr[2]=R, ptr[1]=G, ptr[0]=B
//		RGBA: ptr[0]=R, ptr[1]=G, ptr[2]=B (RBGA never seems to be a thing)
//Finally return the (r,g,b) color as t_color.
//
//"Nearest" sampling so no filtering just picks the closest texel
//see bilinear filtering, sample 4 neighbors and interpolate in UV
//
//use index as placeholder for norm
t_color	sample_image_nearest(const t_tex_img *img, double u, double v)
{
	int		index;
	t_vec2	uvf;
	t_vec2	img_coord;

	if (img == NULL || img->addr == NULL || img->width <= 0 || img->height <= 0)
		return ((t_color){0, 0, 0});
	uvf.x = wrap01(u);
	uvf.y = wrap01(v);
	uvf.y = 1.0 - uvf.y;
	img_coord.x = (int)(uvf.x * (double)(img->width - 1));
	img_coord.y = (int)(uvf.y * (double)(img->height - 1));
	img_coord.x = clamp(img_coord.x, 0, img->width - 1);
	img_coord.y = clamp(img_coord.y, 0, img->height - 1);
	index = img_coord.y * img->line_len + img_coord.x * (img->bpp / 8);
	return (fetch_texel(img, index));
}
