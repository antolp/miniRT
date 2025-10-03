/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/10/04 18:28:10 by epinaud          ###   ########.fr       */
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
# include "shading.h"

# define M_PI 3.14159265358979323846
# define SAMPLE_LVL 7
# define WIDTH 900
# define HEIGHT 700
# define SHADOW_EPS 0.001

// A SUPPR
# include <stdio.h>
# include <sys/time.h>

// typedef enum e_objects_props
// {
// 	PROP_POSITION = 0,
// 	PROP_DIRECTION = 1 << 0,
// 	PROP_COLOUR = 1 << 1,
// 	PROP_RATIO = 1 << 2,
// 	PROP_BRIGHTNESS = 1 << 3,
// 	PROP_PATH = 1 << 4,
// 	PROP_DIAMETER = 1 << 5,
// 	PROP_FOV = 1 << 6,
// 	PROP_AXIS = 1 << 7
// }	t_object_props;

typedef enum e_objects_props
{
	PROP_POSITION,
	PROP_DIRECTION,
	PROP_COLOUR,
	PROP_RATIO,
	PROP_BRIGHTNESS,
	PROP_PATH,
	PROP_DIAMETER,
	PROP_FOV,
	PROP_AXIS
}	t_object_props;

void	parse_rt(char *path);

typedef struct s_hit_info
{
	t_vec3			hit_point;
	t_vec3			normal;
	t_color			hit_color;
	t_object		*object;
	bool			is_outside;
}	t_hit_info;

//complex shadow variable struct for norm
//(one fucking parameter away)
typedef struct s_hit_shadow_var
{
	t_ray		*ray;
	t_object	*ignore;
	double		max_t;
	double		t_min;
	t_hit_info	*hit;
}	t_hit_shadow_var;

//used for per-pixel rendering in MiniLibX
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
	int				shading_flag;
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
bool	intersect_cone(t_object *obj, t_ray *ray, double *t);
bool	intersect_triangle(t_object *obj, t_ray *ray, double *t);

void	get_normal_triangle(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal);
void	get_normal_plane(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal);
void	get_normal_sphere(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal);
void	get_normal_cylinder(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal);
void	get_normal_cone(t_object *obj, t_vec3 *hit_point, t_vec3 *out_normal);

//shading
t_color	shade_pixel(t_ray *ray, t_hit_info *hit, int depth);
bool	is_in_shadow(t_vec3 point, t_vec3 light_pos);
t_color	color_add(t_color a, t_color b);
t_color	color_lerp(t_color c1, t_color c2, double t);
t_color	color_scale_clamped(t_color c, double s);
t_color	color_scale_sat(t_color c, double s);

t_color	compute_diffuse_lighting(t_hit_info *hit);
t_color	compute_specular_highlight(t_material *m, t_hit_info *hit, t_vec3 view_dir);
t_color	compute_reflection(t_material *m, t_ray *ray, t_hit_info *hit, 
			t_color current);
t_ray	compute_reflected_ray(t_ray *ray, t_hit_info *hit);
t_color	compute_refraction(t_material *m, t_ray *ray, t_hit_info *hit,
			int depth);
double	get_fresnel_weight(t_material *m, t_ray *ray, t_hit_info *hit);
t_color	compute_shadow_attenuation(t_vec3 point, t_vec3 light_pos);
double	caustic_gain(double ior, double cos_in, double cos_out);
bool	get_closest_hit_ignoring(t_ray *ray, double max_t,
	t_object *ignore, t_hit_info *hit);


//texture
t_color	get_hit_color(t_hit_info *hit);
bool	get_uv_plane(t_object *obj, t_vec3 *hit, t_vec2 *out_uv);
bool	get_uv_sphere(t_object *obj, t_vec3 *hit, t_vec2 *out_uv);
bool	get_uv_cylinder(t_object *obj, t_vec3 *hit, t_vec2 *out_uv);
bool	get_uv_cone(t_object *obj, t_vec3 *hit, t_vec2 *out_uv);
bool	get_uv_triangle(t_object *obj, t_vec3 *hit, t_vec2 *out_uv);
void	init_cyl_uv_vars(t_cyl_uv_vars *v, t_object *obj);
t_color	get_background_color(t_vec3 dir);


double	clamp01(double x);
double	wrap01(double x);
void	build_basis_from_normal(t_vec3 a, t_vec3 *t, t_vec3 *b);
int	is_near_zero(double x);

t_texture_image	*load_xpm_image(void *mlx, const char *path);
t_color	sample_image_nearest(const t_texture_image *img, double u, double v);



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