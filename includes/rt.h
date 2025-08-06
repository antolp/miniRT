/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include <X11/keysym.h>
# include <X11/X.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <float.h>
# include <fcntl.h>
# include <stdbool.h>
# include <math.h>

# include "mlx.h"
# include "scene.h"


# define M_PI 3.14159265358979323846
# define SAMPLE_LVL 3
# define WIDTH 1000
# define HEIGHT 700

# include "scene.h"


// A SUPPR
# include <stdio.h>
# include <sys/time.h>

typedef struct s_hit_info
{
	t_vec3			hit_point;
	t_vec3			normal;
	t_object		*object;
}	t_hit_info;

//used for pixel rendering in MiniLibX
//see mlx_put_image_to_window() in mlx docs
typedef struct s_img
{
	void	*img;			//image pointer
	char	*addr;			//pixel data
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_img;

typedef enum e_render_mode
{
	RENDER_REALTIME,
	RENDER_HIGH_QUALITY,
	RENDER_TEST
}	t_render_mode;

typedef struct s_renderer
{
	void			*mlx;
	void			*win;
	t_img			img;
	t_render_mode	mode;
	int				resolution_scale;	//potentially useful (antialiasing, real time mode)
	bool			is_rendering;		//(redundant on single threaded logic)
	bool			render_done;
	bool			has_drawn_realtime;
	bool			supersampled;
}	t_renderer;



bool		init_renderer(t_renderer *r, int width, int height, char *title);
bool		init_scene(t_scene *s);
int			key_hook(int keycode, void *param);
t_renderer	*g_renderer(t_renderer *set);
t_scene		*g_scene(t_scene *set);

//renderer
t_color		trace_ray(t_ray *ray, int depth);
t_color		trace_fast_ray(t_ray *ray, int none);
void		put_pixel(t_img *img, int x, int y, t_color color);
int			render_loop(void *param);
void		render_test_frame(int frame);
void		render_full_frame(t_camera_basis *cb);
void		render_supersampled_frame(t_camera_basis *cb, int samples);
void		render_downsampled_frame(t_camera_basis *cb, int block_sizes);
t_ray		create_ray_for_pixel(double x, double y,  t_camera_basis *cb);

//ray-object intersection
bool	get_closest_hit(t_ray *ray, t_hit_info *hit);
bool	intersect_plane(t_object *obj, t_ray *ray, double *t);
bool	intersect_sphere(t_object *obj, t_ray *ray, double *t);
bool	intersect_cylinder(t_object *obj, t_ray *ray, double *t);

void	get_normal_plane(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal);
void	get_normal_sphere(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal);
void	get_normal_cylinder(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal);

//shading
t_color	compute_diffuse_lighting(t_material *mat, t_vec3 point, t_vec3 normal);


//maths
t_vec3		vec_add(t_vec3 a, t_vec3 b);
t_vec3		vec_sub(t_vec3 a, t_vec3 b);
t_vec3		vec_mul(t_vec3 v, double scalar);
double		vec_length(t_vec3 v);
double		vec_length_squared(t_vec3 v);
double		vec_dot(t_vec3 a, t_vec3 b);
t_vec3		vec_cross(t_vec3 a, t_vec3 b);
t_vec3		vec_normalize(t_vec3 v);
t_vec3		vec_reflect(t_vec3 dir, t_vec3 normal);


//debug
void	print_color(char *s, t_color c);
void	print_vec3(char *s, t_vec3 v);


#endif