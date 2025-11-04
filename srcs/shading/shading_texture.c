/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shading_texture.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//pick surface color if texture and enabled
t_color	surface_color(t_hit_info *hit)
{
	unsigned int	flags;

	flags = g_renderer(NULL)->shading_flag;
	if ((flags & FLAG_TEXTURE) != 0u
		&& hit->object->material.texture.type != TEXTURE_NONE)
		return (get_hit_color(hit));
	return (hit->object->material.base_color);
}

//bump from image if bump and enabled
void	apply_bump(t_hit_info *hit)
{
	unsigned int	flags;

	flags = g_renderer(NULL)->shading_flag;
	if ((flags & FLAG_BUMP_MAP) != 0u
		&& hit->object->material.texture.has_bump_maps == true)
		apply_bump_from_image(hit->object, hit->hit_point, &hit->normal);
}
