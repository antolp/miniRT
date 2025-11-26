/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diffuse_lights_helpers.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 03:30:39 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 03:30:56 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_color	color_mul_255(t_color a, t_color b)
{
	t_color	r;

	r.r = (int)fmin(255.0, (double)a.r * (double)b.r / 255.0);
	r.g = (int)fmin(255.0, (double)a.g * (double)b.g / 255.0);
	r.b = (int)fmin(255.0, (double)a.b * (double)b.b / 255.0);
	return (r);
}

void	clamp_color(t_color *c)
{
	c->r = fmin(c->r, 255);
	c->g = fmin(c->g, 255);
	c->b = fmin(c->b, 255);
}
