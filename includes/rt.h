/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/29 18:40:39 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H
# define RT_H

# include <X11/keysym.h>
# include <X11/X.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <stdbool.h>
# include <math.h>
# include <stdio.h>
# include <float.h>

# include "mlx.h"
# include "scene.h"
# include "shading.h"
# include "parser.h"

# define M_PI 3.14159265358979323846
# define SAMPLE_LVL 7
# define WIDTH 900
# define HEIGHT 700

//EPS, epsilon next smallest value next to a value in specified context
//e.g., DBL_EPSILON from limits.h : next smallest value after (double)0.0;
//(thats how i understand it)
# define SHADOW_EPS 0.001

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
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}	t_img;

//could add so
typedef enum e_render_mode
{
	RENDER_EDIT,
	RENDER_HIGH_QUALITY,
	RENDER_TEST,
	RENDER_EDIT_MENU
}	t_render_mode;

typedef enum e_edit_target
{
	EDIT_TARGET_CAMERA,
	EDIT_TARGET_OBJECT,
	EDIT_TARGET_LIGHT
}	t_edit_target;

typedef struct s_edit_state
{
	t_edit_target	target;
	t_list			*current_obj;
	t_list			*current_light;
	double			edit_scale;
}	t_edit_state;

typedef struct s_renderer
{
	void			*mlx;
	void			*win;
	t_img			img;
	t_render_mode	mode;
	t_edit_state	edit;
	int				resolution_scale;
	bool			should_quit;
	bool			is_rendering;
	bool			render_done;
	bool			has_drawn_realtime;
	bool			supersampled;
	int				shading_flag;
	int				render_y;
}	t_renderer;

bool		init_renderer(t_renderer *r, int width, int height, char *title);
int			key_hook(int keycode, void *param);
t_renderer	*g_renderer(t_renderer *set);
t_scene		*g_scene(t_scene *set);
void		clear_minirt(void);

//renderer
t_color		trace_ray(t_ray *ray, int depth);
t_color		trace_fast_ray(t_ray *ray, int none);
void		put_pixel(t_img *img, int x, int y, t_color color);
int			render_loop(void *param);
void		render_test_frame(int frame);
void		render_high_quality_frame(void);
void		render_edit_frame(void);
void		render_supersampled_step(t_camera_basis *cb, int samples);
void		render_downsampled_frame(t_camera_basis *cb, int block_sizes);
t_ray		create_ray_for_pixel(double x, double y, t_camera_basis *cb);

//ray-object intersection
bool		get_closest_hit(t_ray *ray, t_hit_info *hit);

bool		intersect_plane(t_object *obj, t_ray *ray, double *t);
bool		intersect_sphere(t_object *obj, t_ray *ray, double *t);
bool		intersect_cylinder(t_object *obj, t_ray *ray, double *t);
bool		intersect_cone(t_object *obj, t_ray *ray, double *t);
bool		intersect_triangle(t_object *obj, t_ray *ray, double *t);

void		get_normal_triangle(t_object *obj, t_vec3 *hit_point,
				t_vec3 *out_normal);
void		get_normal_plane(t_object *obj, t_vec3 *hit_point,
				t_vec3 *out_normal);
void		get_normal_sphere(t_object *obj, t_vec3 *hit_point,
				t_vec3 *out_normal);
void		get_normal_cylinder(t_object *obj, t_vec3 *hit_point,
				t_vec3 *out_normal);
void		get_normal_cone(t_object *obj, t_vec3 *hit_point,
				t_vec3 *out_normal);

void		get_cap_info(t_cylinder *cyl, int i, t_vec3 *center,
				t_vec3 *normal);
bool		is_point_within_cylinder_height(t_vec3 point, t_cylinder *cyl);
void		compute_quad_cyl(t_quad *q, t_cyl_side_vars v, double radius);
void		compute_quad_cone(t_quad *q, t_cone_side_vars v);

//shading
t_color		shade_pixel(t_ray *ray, t_hit_info *hit, int depth);
t_color		surface_color(t_hit_info *hit);
void		apply_bump(t_hit_info *hit);

t_color		shade_diffuse_fast(t_hit_info *hit);
t_color		compute_ambient_light(t_color c, t_scene *scene);
bool		is_in_shadow(t_vec3 point, t_vec3 light_pos);
t_color		color_add(t_color a, t_color b);
t_color		color_lerp(t_color c1, t_color c2, double t);
t_color		color_scale_clamped(t_color c, double s);
t_color		color_scale_sat(t_color c, double s);

t_color		compute_diffuse_lighting(t_hit_info *hit);
t_color		compute_specular_highlight(t_material *m, t_hit_info *hit,
				t_vec3 view_dir);
t_color		compute_reflection(t_material *m, t_ray *ray, t_hit_info *hit,
				t_color current);
t_ray		compute_reflected_ray(t_ray *ray, t_hit_info *hit);
t_color		compute_refraction(t_material *m, t_ray *ray, t_hit_info *hit,
				int depth);
double		get_fresnel_weight(t_material *m, t_ray *ray, t_hit_info *hit);
t_color		compute_shadow_attenuation(t_vec3 point, t_vec3 light_pos);
double		caustic_gain(double ior, double cos_in, double cos_out);
bool		get_closest_hit_ignoring(t_ray *ray, double max_t,
				t_object *ignore, t_hit_info *hit);

//texture
t_color		get_hit_color(t_hit_info *hit);
bool		get_uv_plane(t_object *obj, t_vec3 *hit, t_vec2 *out_uv);
bool		get_uv_sphere(t_object *obj, t_vec3 *hit, t_vec2 *out_uv);
bool		get_uv_cylinder(t_object *obj, t_vec3 *hit, t_vec2 *out_uv);
bool		get_uv_cone(t_object *obj, t_vec3 *hit, t_vec2 *out_uv);
bool		get_uv_triangle(t_object *obj, t_vec3 *hit, t_vec2 *out_uv);
void		init_cyl_uv_vars(t_cyl_uv_vars *v, t_object *obj);
t_color		get_background_color(t_vec3 dir);

double		clamp01(double x);
double		wrap01(double x);
void		build_basis_from_normal(t_vec3 a, t_vec3 *t, t_vec3 *b);
int			is_near_zero(double x);

t_tex_img	*load_xpm_image(void *mlx, const char *path);
t_color		sample_image_nearest(const t_tex_img *img,
				double u, double v);

//bump maps
void		apply_bump_from_image(t_object *obj, t_vec3 hit_p, t_vec3 *normal);
void		bump_tangent_plane(t_bump_var *c);
void		bump_tangent_triangle(t_bump_var *c);
void		bump_tangent_cylinder(t_bump_var *c);
void		bump_tangent_cone(t_bump_var *c);
void		bump_tangent_sphere(t_bump_var *c);
void		build_tb_plane(t_plane *pl, t_vec3 N, t_vec3 *T, t_vec3 *B);
void		build_tb_triangle_ortho(t_triangle *tr, t_vec3 N,
				t_vec3 *T, t_vec3 *B);
void		build_tb_triangle_fit(t_triangle *tr, t_vec3 *T, t_vec3 *B);
void		build_tb_sphere(t_sphere *sp, t_vec3 N, t_vec3 *T, t_vec3 *B);
int			is_cylinder_cap(t_cylinder *cy, t_vec3 P);
void		build_tb_cylinder_side(t_cylinder *cy, t_vec3 P,
				t_vec3 *T, t_vec3 *B);
int			is_cone_base(t_cone *co, t_vec3 P);
void		build_tb_cone_side(t_cone *co, t_vec3 N, t_vec3 *T, t_vec3 *B);

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
t_vec3		vec_safe_normalize(t_vec3 v, t_vec3 fallback);
double		deg2rad(double angle);

//debug
void		print_scene(void);
void		print_color(char *s, t_color c);
void		print_vec3(char *s, t_vec3 v);
void		print_texture(char *s, t_texture tex);
void		print_camera(t_camera *c);
void		print_help(void);
void		print_light(t_light *light);
void		print_obj(t_object *obj);
void		print_sphere(t_sphere *s);
void		print_plane(t_plane *p);
void		print_cylinder(t_cylinder *cyl);
void		print_cone(t_cone *co);
void		print_triangle(t_triangle *t);

//a trier
t_color		compute_fast_preview(t_ray *ray, t_hit_info *hit);
void		orient_normal(t_ray *ray, t_hit_info *hit);

//=== EDIT MODE ===
int			handle_open_flags_menu(int keycode, t_renderer *r);
int			handle_edit_menu_keys_wrapper(int keycode, t_renderer *r);
void		handle_menu_keys(int keycode, t_renderer *r);
const char	*flag_get_name(int index);
uint32_t	flag_get_bit(int index);

//scene edition
int			handle_scene_edit_active_keys(int keycode, t_cli_var *var);
int			handle_scene_edit_keys(int keycode, t_cli_var *var);
void		editor_cycle_selection(t_renderer *r, t_scene *scene, int keycode);

//misc
int			handle_mode_switch(int keycode, t_renderer *r);
int			handle_quit(int keycode);
int			handle_supersample_toggle(int keycode, t_renderer *r);
t_list		*editor_list_last(t_list *lst);
t_list		*editor_list_prev_wrap(t_list *head, t_list *node);

//camera
int			handle_camera_fov_out(int keycode, t_cli_var *var);
int			handle_camera_fov_in(int keycode, t_cli_var *var);
int			handle_camera_pitch(int keycode, t_cli_var *var);
int			handle_camera_yaw(int keycode, t_cli_var *var);

//print
void		edit_print_sphere(t_object *obj);
void		edit_print_plane(t_object *obj);
void		edit_print_cylinder(t_object *obj);
void		edit_print_cone(t_object *obj);
void		edit_print_triangle(t_object *obj);
void		editor_print_current_object(t_renderer *r);
void		editor_print_current_light(t_renderer *r);
void		editor_print_current(t_renderer *r);
void		print_flags_table(uint32_t mask);

//high-level props move
void		editor_print_move(t_renderer *r, char *dir);
int			handle_props_move_ws(int keycode, t_cli_var *var);
int			handle_props_move_ad(int keycode, t_cli_var *var);
int			handle_props_move_tg(int keycode, t_cli_var *var);

//translation
void		editor_translate(t_cli_var *var, t_vec3 delta);
void		editor_translate_camera(t_cli_var *var, t_vec3 delta);
void		editor_translate_object(t_cli_var *var, t_vec3 delta);
void		editor_translate_light(t_cli_var *var, t_vec3 delta);

//object specific translation
void		editor_translate_sphere(t_sphere *s, t_vec3 delta);
void		editor_translate_plane(t_plane *p, t_vec3 delta);
void		editor_translate_cylinder(t_cylinder *c, t_vec3 delta);
void		editor_translate_cone(t_cone *c, t_vec3 delta);
void		editor_translate_triangle(t_triangle *t, t_vec3 delta);

//high-level props rotation
int			handle_props_rotate_yaw(int keycode, t_cli_var *var);
void		editor_rotate_object_yaw(t_cli_var *var, double angle_deg);
int			handle_props_rotate_pitch(int keycode, t_cli_var *var);
void		editor_rotate_object_pitch(t_cli_var *var, double angle_deg);

//rotation
t_vec3		rotate_yaw(t_vec3 dir, double angle_deg);
t_vec3		vec_rotate_yaw(t_vec3 v, double angle_deg);
t_vec3		rotate_pitch(t_vec3 dir, double angle_deg);
t_vec3		vec_rotate_axis(t_vec3 v, t_vec3 axis, double angle_deg);
t_vec3		editor_triangle_center(t_triangle *tri);

//high-level props property/scale editing
int			handle_props_scale_primary(int keycode, t_cli_var *var);
int			handle_props_scale_secondary(int keycode, t_cli_var *var);

//scaling
void		editor_scale_object_primary(t_cli_var *var, double factor);
void		editor_scale_object_secondary(t_cli_var *var, double factor);

//object specific scaling
void		editor_scale_sphere_radius(t_object *obj, double factor);
void		editor_scale_cylinder_radius(t_object *obj, double factor);
void		editor_scale_cylinder_height(t_object *obj, double factor);
void		editor_scale_cone_height(t_object *obj, double factor);
void		editor_scale_cone_angle(t_object *obj, double factor);

#endif
