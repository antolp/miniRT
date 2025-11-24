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

//NOTE: Valgrind may falsely report uninitialized bytes here 
//consequence of X11 internals writev call
//Known issue, check 42 discord !!! may need a .supp file to feed to valgrind
bool	init_renderer(t_renderer *r, int width, int height, char *title)
{
	r->mlx = mlx_init();
	if (!r->mlx)
		return (0);
	r->win = mlx_new_window(r->mlx, width, height, title);
	if (!r->win)
		return (0);
	r->img.img = mlx_new_image(r->mlx, width, height);
	if (!r->img.img)
		return (0);
	r->img.addr = mlx_get_data_addr(r->img.img,
			&r->img.bits_per_pixel, &r->img.line_length, &r->img.endian);
	if (!r->img.addr)
		return (0);
	r->mode = RENDER_HIGH_QUALITY;
	r->resolution_scale = 1;
	r->is_rendering = 0;
	r->render_done = 0;
	r->supersampled = false;
	r->shading_flag = DEFAULT_FLAGS;
	r->edit.target = EDIT_TARGET_CAMERA;
	r->edit.current_obj = NULL;
	r->edit.current_light = NULL;
	return (1);
}

t_list	*add_content(void *content)
{
	t_list	*node;

	node = malloc(sizeof(t_list));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	return (node);
}





// // --- FEATURE HIGHLIGHT : texture and shape UV mapping ---
// // two reflective spheres
// //
bool	init_scene(t_scene *s)
{
	t_plane			*plane;
	t_sphere		*sphere;
	t_object		*obj;
	t_light			*light;
	t_triangle		*tri;

	s->camera.position = (t_vec3){-67.26, 14.02, 68.40};
	s->camera.direction = vec_normalize((t_vec3){0.58, -0.10, -0.81});
	s->camera.fov = 60;

	t_texture_image	*img;
	img = load_xpm_image(g_renderer(NULL)->mlx, "assets/wood.xpm");

	t_texture_image	*img_sphere;
	img_sphere = load_xpm_image(g_renderer(NULL)->mlx, "assets/2k_earth_daymap.xpm");
	t_texture_image	*img_bump;
	img_bump = load_xpm_image(g_renderer(NULL)->mlx, "assets/bumpgrunge.xpm");
	t_texture_image	*img42;
	img42 = load_xpm_image(g_renderer(NULL)->mlx, "assets/42_500_1.xpm");
	// img42 = load_xpm_image(g_renderer(NULL)->mlx, "wrongpath.xpm");

	t_checkerboard *cb; 
	cb = malloc(sizeof(t_checkerboard));
	if (!cb)
		return (false);
	cb->color1 = (t_color){255, 255, 255};
	cb->color2 = (t_color){50, 50, 50};
	cb->scale_u = 0.1;
	cb->scale_v = 0.1;

	t_checkerboard *cb1; 
	cb1 = malloc(sizeof(t_checkerboard));
	if (!cb1)
		return (false);
	cb1->color1 = (t_color){255, 255, 255};
	cb1->color2 = (t_color){50, 50, 50};
	cb1->scale_u = 13;
	cb1->scale_v = 13;

	//skybox
	// s->skybox.type = TEXTURE_NONE;
	// s->skybox.data = NULL;
	t_texture_image *sky = load_xpm_image(g_renderer(NULL)->mlx, "assets/autumn_field_puresky_8k.xpm");
	s->skybox.type = TEXTURE_IMAGE;
	s->skybox.data = sky;

	s->is_rendering = false;
	s->objects = NULL;
	s->lights = NULL;

	// // ---------- SPHERE ----------
	// //mirror red smooth "metallic"
	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return (false);
	sphere->center = (t_vec3){-31, 7.2, 20};
	sphere->radius = 7.2;
	// sphere->center = (t_vec3){12, 20, 35};
	// sphere->radius = 7.7;
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_SPHERE;
	obj->shape = sphere;
	obj->material.base_color = (t_color){255, 60, 60};
	obj->material.reflectivity = 1;
	obj->material.ior = 0.2;
	obj->material.shininess = 256;
	obj->material.specular_strength = 1;
	// obj->material.refractivity = 0.05;
	obj->material.refractivity = 0;
	// obj->material.refractivity = 0;
	// obj->material.texture.type = TEXTURE_NONE;
	// obj->material.texture.type = TEXTURE_CHECKER;
	// obj->material.texture.data = cb_red;
	obj->material.texture.type = TEXTURE_NONE;
	obj->material.texture.data = NULL;
	obj->material.texture.has_bump_maps = false;
	obj->material.texture.bumps_data = NULL;
	obj->material.bump_strength = 1;
	obj->intersect = intersect_sphere;
	obj->get_normal = get_normal_sphere;
	obj->get_uv = get_uv_sphere;
	ft_lstadd_back(&s->objects, add_content(obj));

	// ---------- SPHERE ----------
	//refractive, transmissive sphere
	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return (false);
	sphere->center = (t_vec3){-50.97, 7.7, 23.76};
	sphere->radius = 7.7;
	// sphere->center = (t_vec3){12, 20, 35};
	// sphere->radius = 7.7;
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_SPHERE;
	obj->shape = sphere;
	obj->material.base_color = (t_color){190, 210, 255};
	obj->material.reflectivity = 1;
	obj->material.ior = 1.00001;
	obj->material.shininess = 32;
	obj->material.specular_strength = 1;
	obj->material.refractivity = 0.25;
	// obj->material.refractivity = 0;
	// obj->material.texture.type = TEXTURE_NONE;
	// obj->material.texture.type = TEXTURE_CHECKER;
	// obj->material.texture.data = cb_red;
	obj->material.texture.type = TEXTURE_NONE;
	obj->material.texture.data = NULL;
	obj->material.texture.has_bump_maps = false;
	obj->material.texture.bumps_data = NULL;
	// obj->material.texture.has_bump_maps = true;
	// obj->material.texture.bumps_data = img_bump;
	obj->material.bump_strength = 1;
	obj->intersect = intersect_sphere;
	obj->get_normal = get_normal_sphere;
	obj->get_uv = get_uv_sphere;
	ft_lstadd_back(&s->objects, add_content(obj));

	// // ---------- TRIANGLE SQUARE IMAGE MAPPING ----------
	// // could write a small function that generates the right coordinates
	// // for each triangles depending of the image ratio
	// // // ---------- triangle1 ----------
	tri = malloc(sizeof(t_triangle));
	if (!tri)
		return (false);
	// tri->p0 = (t_vec3){0, 0, 0};
	// tri->p1 = (t_vec3){10, 0, 0};
	// tri->p2 = (t_vec3){0, 10, 0};
	tri->p0 = (t_vec3){-30, 0, 50};
	tri->p1 = (t_vec3){-55, 0, 50};
	tri->p2 = (t_vec3){-30, 25, 50};
	tri->uv_mode = TRI_UV_FIT;
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_TRIANGLE;
	obj->shape = tri;
	obj->material.base_color = (t_color){10, 255, 155};
	obj->material.reflectivity = 1;
	obj->material.shininess = 20;
	obj->material.specular_strength = 1.3;
	obj->material.ior = 1.035;
	obj->material.refractivity = 0.9;
	obj->material.texture.type = TEXTURE_NONE;
	obj->material.texture.data = NULL;
	// obj->material.texture.type = TEXTURE_CHECKER;
	// obj->material.texture.data = cb1;
	// obj->material.texture.has_bump_maps = false;
	// obj->material.texture.bumps_data = NULL;
	obj->material.texture.has_bump_maps = true;
	obj->material.texture.bumps_data = img_bump;
	obj->material.bump_strength = 1;
	obj->intersect = intersect_triangle;
	obj->get_normal = get_normal_triangle;
	obj->get_uv = get_uv_triangle;
	ft_lstadd_back(&s->objects, add_content(obj));
	// ---------- triangle2 ----------
	tri = malloc(sizeof(t_triangle));
	if (!tri)
		return (false);
	// tri->p0 = (t_vec3){10, 10, 0};
	// tri->p1 = (t_vec3){0, 10, 0};
	// tri->p2 = (t_vec3){10, 0, 0};
	tri->p0 = (t_vec3){-55, 25, 50};
	tri->p1 = (t_vec3){-30, 25, 50};
	tri->p2 = (t_vec3){-55, 0, 50};
	tri->uv_mode = TRI_UV_FIT_OPPOSITE;
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_TRIANGLE;
	obj->shape = tri;
	obj->material.base_color = (t_color){10, 255, 155};
	obj->material.reflectivity = 1;
	obj->material.shininess = 20;
	obj->material.specular_strength = 1.3;
	obj->material.ior = 1.035;
	obj->material.refractivity = 0.9;
	// obj->material.texture.type = TEXTURE_NONE;
	// obj->material.texture.type = TEXTURE_CHECKER;
	// obj->material.texture.data = cb;
	obj->material.texture.type = TEXTURE_NONE;
	obj->material.texture.data = NULL;
	// obj->material.texture.has_bump_maps = false;
	// obj->material.texture.bumps_data = NULL;
	obj->material.texture.has_bump_maps = true;
	obj->material.texture.bumps_data = img_bump;
	obj->material.bump_strength = 1;
	obj->intersect = intersect_triangle;
	obj->get_normal = get_normal_triangle;
	obj->get_uv = get_uv_triangle;
	ft_lstadd_back(&s->objects, add_content(obj));

	// ---------- PLANE ----------
	plane = malloc(sizeof(t_plane));
	if (!plane)
		return (false);
	plane->point = (t_vec3){0, 0, 0};
	plane->normal = (t_vec3){0, 1, 0};
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_PLANE;
	obj->shape = plane;
	obj->material.base_color = (t_color){255, 255, 255};
	obj->material.reflectivity = 1;
	obj->material.ior = 1.001; 
	obj->material.specular_strength = 0;
	obj->material.shininess = 0;
	obj->material.refractivity = 0;
	obj->material.texture.type = TEXTURE_CHECKER;
	obj->material.texture.data = cb;
	obj->material.texture.has_bump_maps = false;
	obj->material.texture.bumps_data = NULL;
	obj->material.bump_strength = 0;
	obj->intersect = intersect_plane;
	obj->get_normal = get_normal_plane;
	obj->get_uv = get_uv_plane;
	ft_lstadd_back(&s->objects, add_content(obj));

	// ---------- LIGHT ----------
	light = malloc(sizeof(t_light));
	if (!light)
		return (false);
	// light->position = (t_vec3){20, 30, -30};
	light->position = (t_vec3){38.39, 44.66, 82.46};
	light->intensity = 0.8;
	light->color = (t_color){255, 255, 255};
	ft_lstadd_back(&s->lights, add_content(light));
		light = malloc(sizeof(t_light));
	if (!light)
		return (false);
	light->position = (t_vec3){10, 30, 30};
	light->intensity = 0.1;
	light->color = (t_color){255, 200, 200};
	// light->color = (t_color){255, 200, 200};
	ft_lstadd_back(&s->lights, add_content(light));

	s->ambient_color = (t_color){255, 255, 255};
	s->ambient_ratio = 0.25;
	s->background_color = (t_color){100, 100, 200};
	return (1);
}
