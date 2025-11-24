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


//init_camera_basis()
//this one is not self evident, basically initializes a bunch of infos about
//the camera that are only useful for to get the rays directions
//forward, right and up vectors are useful to compute the new "camera space",
//in which the rays can be meaningfully launched.
//Computed using "world_up", the world space Y axis.
//
//If forward is too close to the Y axis in world space, fallback to
//X axis in order to avoid null division.
//
//scale is the scaling factor for ray projection into world space.
//top view (angles make no sense) :
//          .
//         /|  (screen plane placed at Z = 1)
//        / |
//       /  |  h = half height of screen plane (on the y axis)
//      /   |
//     /    |       ^
// C  /_____|       | Y axis
//       1          o ------>  Z axis
//
//basic trigonometry :
//	tan(d) = opposite / adjacent
//	tan(fov / 2) = h / 1 
//	h = tan(fov / 2)
//that's how FOV works
static void	init_camera_basis(t_camera_basis *cb, t_camera *cam,
		int width, int height)
{
	t_vec3	world_up;

	world_up = (t_vec3){0, 1, 0};
	cb->forward = vec_normalize(cam->direction);
	if (fabs(vec_dot(cb->forward, world_up)) > 0.999)
		world_up = (t_vec3){1, 0, 0};
	cb->right = vec_normalize(vec_cross(world_up, cb->forward));
	cb->up = vec_cross(cb->forward, cb->right);
	cb->aspect_ratio = (double)width / (double)height;
	cb->scale = tan(cam->fov * M_PI / 360.0);
}


static void	render_full_frame(t_camera_basis *cb)
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
		mlx_put_image_to_window(r->mlx, r->win, r->img.img, 0, 0);
		y++;
	}
}

void	render_high_quality_frame(void)
{
	t_scene			*s;
	t_renderer		*r;
	t_camera_basis	cb;

	r = g_renderer(NULL);
	s = g_scene(NULL);
	if (r->is_rendering || r->render_done)
		return ;
	init_camera_basis(&cb, &s->camera, WIDTH, HEIGHT);
	if (r->supersampled == false)
		render_full_frame(&cb);
	if (r->supersampled == true)
		render_supersampled_frame(&cb, SAMPLE_LVL);
	r->is_rendering = false;
	r->render_done = true;
	printf("HQ Render Finished. Press E to edit.\n");
}

void	render_edit_frame()
{
	t_scene			*s;
	t_renderer		*r;
	t_camera_basis	cb;

	r = g_renderer(NULL);
	s = g_scene(NULL);
	if (!r->has_drawn_realtime)
	{
		init_camera_basis(&cb, &s->camera, WIDTH, HEIGHT);
		render_downsampled_frame(&cb, 3);
		mlx_put_image_to_window(r->mlx, r->win, r->img.img, 0, 0);
		r->has_drawn_realtime = true;
	}
}
