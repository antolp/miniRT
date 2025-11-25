#include "rt.h"

static t_color	supersample_sample_row(t_vec2 pixel, t_supersample_var *var, 
	int i)
{
	t_color	acc;
	t_vec2	uv;
	t_ray	ray;
	t_color	sample;
	int		j;

	acc = (t_color){0, 0, 0};
	j = 0;
	while (j < var->samples)
	{
		uv.x = pixel.x + ((double)i + 0.5) * var->offset;
		uv.y = pixel.y + ((double)j + 0.5) * var->offset;
		ray = create_ray_for_pixel(uv.x, uv.y, var->cb);
		sample = trace_ray(&ray, 10);
		acc.r += sample.r;
		acc.g += sample.g;
		acc.b += sample.b;
		j++;
	}
	return (acc);
}

static t_color	supersample_accumulate(t_vec2 pixel, t_supersample_var *var)
{
	t_color	acc;
	t_color	row_color;
	int		i;

	acc = (t_color){0, 0, 0};
	i = 0;
	while (i < var->samples)
	{
		row_color = supersample_sample_row(pixel, var, i);
		acc.r += row_color.r;
		acc.g += row_color.g;
		acc.b += row_color.b;
		i++;
	}
	return (acc);
}

static t_color	supersample_pixel(t_vec2 pixel, t_camera_basis *cb, int samples)
{
	t_supersample_var	var;
	t_color				acc;
	int					total;

	var.cb = cb;
	var.samples = samples;
	var.offset = 1.0 / (double)samples;
	acc = supersample_accumulate(pixel, &var);
	total = samples * samples;
	acc.r /= total;
	acc.g /= total;
	acc.b /= total;
	return (acc);
}

static void	render_supersampled_row(int y, t_camera_basis *cb,
			int samples, t_renderer *r)
{
	int		x;
	t_color	color;
	t_vec2	pixel;

	x = 0;
	while (x < WIDTH)
	{
		pixel.x = (double)x;
		pixel.y = (double)y;
		color = supersample_pixel(pixel, cb, samples);
		put_pixel(&r->img, x, y, color);
		x++;
	}
}

void	render_supersampled_frame(t_camera_basis *cb, int samples)
{
	int			y;
	t_renderer	*r;

	r = g_renderer(NULL);
	y = 0;
	while (y < HEIGHT)
	{
		render_supersampled_row(y, cb, samples, r);
		if (y % 50 == 0)
			printf("%d%%\n", (y * 100 / HEIGHT));
		mlx_put_image_to_window(r->mlx, r->win, r->img.img, 0, 0);
		y++;
	}
}
