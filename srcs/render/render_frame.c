/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_frame.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_color	supersample_pixel(int px, int py, t_camera_basis *cb, int samples)
{
	int			i;
	int			j;
	double		offset;
	t_color		acc;
	t_color		sample;
	t_ray		ray;

	acc = (t_color){0, 0, 0};
	offset = 1.0 / samples;
	i = 0;
	while (i < samples)
	{
		j = 0;
		while (j < samples)
		{
			double u = px + (i + 0.5) * offset;
			double v = py + (j + 0.5) * offset;
			ray = create_ray_for_pixel(u, v, cb);
			sample = trace_ray(&ray, 10);
			acc.r += sample.r;
			acc.g += sample.g;
			acc.b += sample.b;
			j++;
		}
		i++;
	}
	acc.r /= (samples * samples);
	acc.g /= (samples * samples);
	acc.b /= (samples * samples);
	return (acc);
}

void	render_supersampled_frame(t_camera_basis *cb, int samples)
{
	int			x;
	int			y;
	t_color		color;
	t_renderer	*r;

	r = g_renderer(NULL);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			color = supersample_pixel(x, y, cb, samples);
			put_pixel(&r->img, x, y, color);
			x++;
		}
		if (y % 50 == 0)
			printf("%d%%\n", (y * 100 / HEIGHT));
		y++;
	}
}


void	render_full_frame(t_camera_basis *cb)
{
	int			x;
	int			y;
	t_ray		ray;
	t_color		color;
	t_renderer	*r;

	y = 0;
	r = g_renderer(NULL);
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			ray = create_ray_for_pixel(x, y, cb);
			color = trace_ray(&ray, 10);
			put_pixel(&r->img, x, y, color);
			x++;
		}
		if (r->mode == RENDER_HIGH_QUALITY && y % 50 == 0)
			printf("%d%%\n", (y * 100 / HEIGHT));
		y++;
	}
}


void	render_downsampled_frame(t_camera_basis *cb, int block_size)
{
	t_renderer	*r;
	t_ray		ray;
	t_color		color;
	int			x;
	int			y;
	int			i;
	int			j;

	r = g_renderer(NULL);
	y = 0;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			ray = create_ray_for_pixel(x, y, cb);
			color = trace_fast_ray(&ray, 1);
			j = 0;
			while (j < block_size && y + j < HEIGHT)
			{
				i = 0;
				while (i < block_size && x + i < WIDTH)
				{
					put_pixel(&r->img, x + i, y + j, color);
					i++;
				}
				j++;
			}
			x += block_size;
		}
		y += block_size;
	}
}
