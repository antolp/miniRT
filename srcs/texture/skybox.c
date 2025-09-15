/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//mapping from inside a sphere is surprisingly straight forward
static void	sky_equirect_uv(t_vec3 d, double *u, double *v)
{
	double not_u;
	double not_v;
	double len;

	len = sqrt(vec_dot(d, d));
	if (len > 0.0)
		d = vec_mul(d, 1.0 / len);
	not_u = 0.5 + atan2(d.z, d.x) / (2.0 * M_PI);
	not_v = 0.5 + asin(d.y) / M_PI;
	*u = wrap01(not_u);
	*v = clamp01(not_v);
}

t_color	sample_skybox_dir(const t_texture_image *img, t_vec3 dir)
{
	double	u;
	double	v;

	if (img == NULL)
		return ((t_color){0, 0, 0});
	sky_equirect_uv(dir, &u, &v);
	//let user rotate equator in the futur if i ever implement UV edition
	return (sample_image_nearest(img, u, v));
}

t_color	get_background_color(t_vec3 dir)
{
	t_scene *sc;
	t_texture_image *env;

	sc = g_scene(NULL);
	if (sc->skybox.type == TEXTURE_IMAGE && sc->skybox.data != NULL)
	{
		env = (t_texture_image *)sc->skybox.data;
		return (sample_skybox_dir(env, dir));
	}
	return (sc->background_color);
}
