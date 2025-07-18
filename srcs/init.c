/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//hard-coded for now
bool	init_scene(t_scene *s)
{
	(void)s;
	return (0);
}


//NOTE: Valgrind may falsely report uninitialized bytes here 
//consequence of X11 internals writev call
//Known issue, check discord !!! may need a .supp file to feed to valgrind
bool	init_renderer(t_renderer *r, int width, int height, char *title)
{
	r->mlx = mlx_init();
	if (!r->mlx)
		return (0);
	r->win = mlx_new_window(r->mlx, width, height, title);
	if (!r->win)
		return (0);
	r->img.img = mlx_new_image(r->mlx, width, height);
	if (!r->img.img)
		return (0);
	r->img.addr = mlx_get_data_addr(r->img.img,
			&r->img.bits_per_pixel, &r->img.line_length, &r->img.endian);
	if (!r->img.addr)
		return (0);
	r->mode = RENDER_DEBUG;
	r->resolution_scale = 1;
	r->is_rendering = 0;
	return (1);
}