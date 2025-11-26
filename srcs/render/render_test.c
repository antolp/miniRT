/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 03:22:30 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//uses sine wave function to generate a smooth color intensity in [0, 255]
//maps sinus output value from [-1, 1] back to [0, 1]
static int	color_cycle_component(double t, double phase)
{
	double	value;

	value = sin(t + phase) * 0.5 + 0.5;
	return ((int)(value * 255));
}

//returns a color based on the frame index, creates the "shift (raibow effect)"
//value of each color channels correspond to thirds of sine wave
//(2π/3 = 2.09 etc)
static t_color	cycle_color(int frame)
{
	double	t;
	double	scale;
	t_color	color;

	scale = 0.03;
	t = (double)frame * scale;
	color.r = color_cycle_component(t, 0.0);
	color.g = color_cycle_component(t, 2.09);
	color.b = color_cycle_component(t, 4.19);
	return (color);
}

//gathers all pixel colors in a frame buffer
//pass said buffer to x11 server
void	render_test_frame(int frame)
{
	t_renderer	*r;
	t_color		color;
	int			x;
	int			y;

	r = g_renderer(NULL);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			color = cycle_color(frame + x + y);
			put_pixel(&r->img, x, y, color);
			x++;
		}
		y++;
	}
	mlx_put_image_to_window(r->mlx, r->win, r->img.img, 0, 0);
}
