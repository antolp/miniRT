/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
#define PIXEL_CENTER_OFFSET 0.5

void	put_pixel(t_img *img, int x, int y, t_color color)
{
	int	offset;

	offset = (y * img->line_length) + (x * (img->bits_per_pixel / 8));
	img->addr[offset + 0] = color.b;
	img->addr[offset + 1] = color.g;
	img->addr[offset + 2] = color.r;
}

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
void	init_camera_basis(t_camera_basis *cb, t_camera *cam,
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


//create_ray_for_pixel()
//Given a pixel position on the screen (x, y) and a camera basis, computes the
//ray starting at the camera origin that goes through that pixel on the image 
//plane. The returned ray starts at camera position and ends at computed point.
//
//goal is to map pixel coordinates (x, y) into Normalized Device Coordinates :
//	- ndc_x maps [0, WIDTH] to [-1, 1] (left to right)
//	- ndc_y maps [0, HEIGHT] to [1, -1] (top to bottom)
//practical example :
//	- left edge of the screen (x = 0) -> ndc_x = -1
//	- bottom edge (y = HEIGHT) -> ndc_y = -1
//
//then multiply by aspect ratio and FOV (cb->scale) to account
//for perspective distortion and image plane size.
//
//compute offset from the camera forward direction using
//the right and up basis vectors scaled by ndc_x and ndc_y.
//
//add offset to the forward direction to get full direction vector,
//finally normalize it to get the ray direction.
//
//Ressources :
//https://en.wikipedia.org/wiki/Linear_map
//https://learnopengl.com/Getting-started/Coordinate-Systems
//https://www.scratchapixel.com/lessons/3d-basic-rendering/
//ray-tracing-generating-camera-rays/standard-coordinate-systems.html
t_ray	create_ray_for_pixel(double x, double y, 
		t_camera_basis *cb)
{
	double	ndc_x;
	double	ndc_y;
	t_vec3	offset;
	t_vec3	dir;
	t_scene	*s;

	s = g_scene(NULL);
	ndc_x = (2.0 * (x + PIXEL_CENTER_OFFSET) / WIDTH - 1.0)
		* cb->aspect_ratio * cb->scale;
	ndc_y = (1.0 - 2.0 * (y + PIXEL_CENTER_OFFSET) / HEIGHT) * cb->scale;
	offset = vec_add(vec_mul(cb->right, ndc_x),
			vec_mul(cb->up, ndc_y));
	dir = vec_normalize(vec_add(cb->forward, offset));
	return ((t_ray){s->camera.position, dir});
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
	mlx_put_image_to_window(r->mlx, r->win, r->img.img, 0, 0);
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
		//here, render a low detail, fast frame by moduling ray_trace loop call
		//e.g. bigger pixel, minimal to no shading, etc
		//mlx_put_image_to_window(r->mlx, r->win, r->img.img, 0, 0);
		init_camera_basis(&cb, &s->camera, WIDTH, HEIGHT);
		render_downsampled_frame(&cb, 3);
		mlx_put_image_to_window(r->mlx, r->win, r->img.img, 0, 0);
		r->has_drawn_realtime = true;
	}
}

int	render_loop(void *param)
{
	static int	frame = 0;
	t_renderer	*r;

	(void)param;
	r = g_renderer(NULL);
	if (r->mode == RENDER_TEST)
		render_test_frame(frame++);
	else if (r->mode == RENDER_EDIT)
		render_edit_frame();
	else if (r->mode == RENDER_HIGH_QUALITY)
		render_high_quality_frame();
	return (0);
}
