#include "rt.h"

void	render_block_pixels(t_vec2 origin, int block_size,
			t_color color, t_renderer *r)
{
	t_vec2	offset;

	offset.y = 0.0;
	while (offset.y < (double)block_size
		&& origin.y + offset.y < (double)HEIGHT)
	{
		offset.x = 0.0;
		while (offset.x < (double)block_size
			&& origin.x + offset.x < (double)WIDTH)
		{
			put_pixel(&r->img,
				(int)(origin.x + offset.x),
				(int)(origin.y + offset.y),
				color);
			offset.x += 1.0;
		}
		offset.y += 1.0;
	}
}

void	render_downsampled_row(int y, int block_size,
			t_camera_basis *cb, t_renderer *r)
{
	int		x;
	t_ray	ray;
	t_color	color;
	t_vec2	origin;

	x = 0;
	while (x < WIDTH)
	{
		ray = create_ray_for_pixel(x, y, cb);
		color = trace_fast_ray(&ray, 1);
		origin.x = (double)x;
		origin.y = (double)y;
		render_block_pixels(origin, block_size, color, r);
		x += block_size;
	}
}

void	render_downsampled_frame(t_camera_basis *cb, int block_size)
{
	t_renderer	*r;
	int			y;

	r = g_renderer(NULL);
	y = 0;
	while (y < HEIGHT)
	{
		render_downsampled_row(y, block_size, cb, r);
		y += block_size;
	}
}
