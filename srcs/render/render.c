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

void	put_pixel(t_img *img, int x, int y, t_color color)
{
	int	offset;

	offset = (y * img->line_length) + (x * (img->bits_per_pixel / 8));
	img->addr[offset + 0] = color.b;
	img->addr[offset + 1] = color.g;
	img->addr[offset + 2] = color.r;
}

//en chantier
int	render_loop(void *param)
{
	static int	frame = 0;
	t_renderer	*r;

	(void)param;
	r = g_renderer(NULL);

	if (r->mode == RENDER_TEST)
	{
		render_test_frame(frame++);
		return (0);
	}
	if (r->mode == RENDER_REALTIME)
	{
		mlx_put_image_to_window(r->mlx, r->win, r->img.img, 0, 0);
		return (0);
	}
	if (r->mode == RENDER_HIGH_QUALITY)
	{
		if (!r->is_rendering && !r->render_done)
		{
			r->is_rendering = true;
			printf("Starting HQ Render...\n");

			usleep(100000);
			printf("10%%\n");
			usleep(100000);
			printf("20%%\n");
			usleep(100000);
			printf("30%%\n");
			usleep(100000);
			printf("40%%\n");
			usleep(100000);
			printf("50%%\n");
			usleep(100000);
			printf("60%%\n");
			usleep(100000);
			printf("70%%\n");
			usleep(100000);
			printf("80%%\n");
			usleep(100000);
			printf("90%%\n");
			usleep(100000);
			r->is_rendering = false;
			r->render_done = true;
			printf("HQ Render Finished. Press E to edit.\n");

			mlx_put_image_to_window(r->mlx, r->win, r->img.img, 0, 0);
		}
	}
	return (0);
}


