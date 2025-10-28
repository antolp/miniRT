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
	img_bump = load_xpm_image(g_renderer(NULL)->mlx, "assets/wood.xpm");
	t_texture_image	*img42;
	img42 = load_xpm_image(g_renderer(NULL)->mlx, "assets/42_500.xpm");

	t_checkerboard *cb; 
	cb = malloc(sizeof(t_checkerboard));
	if (!cb)
		return (false);
	cb->color1 = (t_color){255, 255, 255};
	cb->color2 = (t_color){50, 50, 50};
	cb->scale_u = 0.1;
	cb->scale_v = 0.1;

	//skybox
	// s->skybox.type = TEXTURE_NONE;
	t_texture_image *sky = load_xpm_image(g_renderer(NULL)->mlx, "assets/autumn_field_puresky_8k.xpm");
	s->skybox.type = TEXTURE_IMAGE;
	s->skybox.data = sky;

	s->is_rendering = false;
	s->objects = NULL;
	s->lights = NULL;

	// ---------- SPHERE ----------
	//refractive rough bump map
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
	obj->material.base_color = (t_color){60, 60, 255};
	obj->material.reflectivity = 1;
	obj->material.ior = 0.15;
	obj->material.shininess = 32;
	obj->material.specular_strength = 1;
	// obj->material.refractivity = 0.05;
	obj->material.refractivity = 0;
	// obj->material.refractivity = 0;
	// obj->material.texture.type = TEXTURE_NONE;
	// obj->material.texture.type = TEXTURE_CHECKER;
	// obj->material.texture.data = cb_red;
	obj->material.texture.type = TEXTURE_NONE;
	obj->material.texture.data = NULL;
	// obj->material.bump_maps.type = TEXTURE_IMAGE;
	// obj->material.bump_maps.data = img;
	obj->material.bump_maps.type = TEXTURE_IMAGE;
	obj->material.bump_maps.data = img;
	obj->material.bump_strength = 1;
	obj->intersect = intersect_sphere;
	obj->get_normal = get_normal_sphere;
	obj->get_uv = get_uv_sphere;
	ft_lstadd_back(&s->objects, add_content(obj));

	// ---------- SPHERE ----------
	//slightly refractive, transmissive sphere
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
	obj->material.base_color = (t_color){255, 255, 255};
	obj->material.reflectivity = 1;
	obj->material.ior = 1.008;
	obj->material.shininess = 32;
	obj->material.specular_strength = 1;
	obj->material.refractivity = 0.05;
	// obj->material.refractivity = 0;
	// obj->material.texture.type = TEXTURE_NONE;
	// obj->material.texture.type = TEXTURE_CHECKER;
	// obj->material.texture.data = cb_red;
	obj->material.texture.type = TEXTURE_NONE;
	obj->material.texture.data = NULL;
	// obj->material.bump_maps.type = TEXTURE_NONE;
	// obj->material.bump_maps.data = NULL;
	obj->material.bump_maps.type = TEXTURE_IMAGE;
	obj->material.bump_maps.data = img_bump;
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
	obj->material.texture.type = TEXTURE_IMAGE;
	obj->material.texture.data = img42;
	obj->material.bump_maps.type = TEXTURE_IMAGE;
	obj->material.bump_maps.data = img_bump;
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
	obj->material.texture.type = TEXTURE_IMAGE;
	obj->material.texture.data = img42;
	obj->material.bump_maps.type = TEXTURE_IMAGE;
	obj->material.bump_maps.data = img_bump;
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
	obj->material.bump_maps.type = TEXTURE_NONE;
	obj->material.bump_maps.data = NULL;
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
	s->background_color = (t_color){10, 10, 40};
	return (1);
}





// //--- FEATURE HIGHLIGHT : three material properties ---
// //
// //refraction based on refractivity and ior
// //refractivity only defines how tinted the glass is, lower is more tinted
// //0.0001 means clear glass, 1 means completely tinted  
// //	slightly disruptive glass : ior = 1.001 to 1,03;
// //this fakes the effect of an empty object made of glass.
// //
// //other values are buggy and thus experimental, but still interesting
// //	completely solid glass ball : ior = 1.3 to 1.52;
// //very dense "crystal" : ior = 1.8 to 1.9. works best with squarish shape
// //a solid glass ball or cylinder will act like a lense, that's the reason
// //the image formed by a sphere of ior 1.52 looks "inverted".
// bool	init_scene(t_scene *s)
// {
// 	t_sphere	*sphere;
// 	t_cylinder	*cylinder;
// 	t_plane		*plane;
// 	t_object	*obj;
// 	t_light		*light;

// 	s->camera.position = (t_vec3){-1, 6.7, -6.9};
// 	s->camera.direction = (t_vec3){-0.51, -0.51, 1};
// 	s->camera.fov = 70;

// 	s->is_rendering = false;
// 	s->objects = NULL;
// 	s->lights = NULL;

// 	//sphere 1, transparent, refractive slightly disruptive sphere
// 	sphere = malloc(sizeof(t_sphere));
// 	if (!sphere)
// 		return (false);
// 	sphere->center = (t_vec3){-3, 3, 2};
// 	sphere->radius = 3;
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_SPHERE;
// 	obj->shape = sphere;
// 	obj->material.base_color = (t_color){10, 255, 155};
// 	obj->material.reflectivity = 1;
// 	obj->material.shininess = 20;
// 	obj->material.specular_strength = 1.3;
// 	obj->material.ior = 1.015;
// 	obj->material.refractivity = 0.7;
// 	obj->intersect = intersect_sphere;
// 	obj->get_normal = get_normal_sphere;
// 	ft_lstadd_back(&s->objects, add_content(obj));
	
// 	//another glass sphere
// 	sphere = malloc(sizeof(t_sphere));
// 	if (!sphere)
// 		return (false);
// 	sphere->center = (t_vec3){-3, 3.9, -3};
// 	sphere->radius = 1.81;
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_SPHERE;
// 	obj->shape = sphere;
// 	obj->material.base_color = (t_color){55, 55, 255};
// 	obj->material.reflectivity = 1;
// 	obj->material.shininess = 20;
// 	obj->material.specular_strength = 1.3;
// 	obj->material.ior = 1.015;
// 	obj->material.refractivity = 0.01;
// 	obj->intersect = intersect_sphere;
// 	obj->get_normal = get_normal_sphere;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	//sphere 2, shiny, reflective sphere
// 	sphere = malloc(sizeof(t_sphere));
// 	if (!sphere)
// 		return (false);
// 	sphere->center = (t_vec3){-10, 3, 2};
// 	sphere->radius = 3;
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_SPHERE;
// 	obj->shape = sphere;
// 	obj->material.base_color = (t_color){255, 10, 155};
// 	obj->material.reflectivity = 0.9;
// 	obj->material.shininess = 20;
// 	obj->material.specular_strength = 1.3;
// 	obj->material.ior = 2.3;
// 	obj->material.refractivity = 0;
// 	obj->intersect = intersect_sphere;
// 	obj->get_normal = get_normal_sphere;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	//cynlinder, reflective perfect mirror
// 	cylinder = malloc(sizeof(t_cylinder));
// 	if (!cylinder)
// 		return (false);
// 	cylinder->center = (t_vec3){-13, -10, -33};
// 	cylinder->axis = vec_normalize((t_vec3){0.2, 1, 0.1}),
// 	cylinder->radius = 12;
// 	cylinder->height = 30;
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_CYLINDER;
// 	obj->shape = cylinder;
// 	obj->material.base_color = (t_color){75, 50, 255};
// 	obj->material.reflectivity = 1;
// 	obj->material.shininess = 20;
// 	obj->material.specular_strength = 1.3;
// 	obj->material.ior = 0.2;
// 	obj->material.refractivity = 0;
// 	obj->intersect = intersect_cylinder;
// 	obj->get_normal = get_normal_cylinder;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	//sphere 3, mate shiny sphere, no reflection
// 	sphere = malloc(sizeof(t_sphere));
// 	if (!sphere)
// 		return (false);
// 	sphere->center = (t_vec3){-17, 3, 2};
// 	sphere->radius = 3;
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_SPHERE;
// 	obj->shape = sphere;
// 	obj->material.base_color = (t_color){255, 255, 255};
// 	obj->material.reflectivity = 0;
// 	obj->material.shininess = 20;
// 	obj->material.specular_strength = 1.3;
// 	obj->material.ior = 1.02;
// 	obj->material.refractivity = 0;
// 	obj->intersect = intersect_sphere;
// 	obj->get_normal = get_normal_sphere;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	// ---------- PLANE ----------
// 	plane = malloc(sizeof(t_plane));
// 	if (!plane)
// 		return (false);
// 	plane->point = (t_vec3){0, -5, 0};
// 	plane->normal = (t_vec3){0, 1, 0};
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_PLANE;
// 	obj->shape = plane;
// 	obj->material.base_color = (t_color){255, 255, 255};
// 	obj->material.reflectivity = 1;
// 	obj->material.ior = 1;
// 	obj->material.specular_strength = 0;
// 	obj->material.shininess = 0;
// 	obj->intersect = intersect_plane;
// 	obj->get_normal = get_normal_plane;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	// ---------- LIGHT ----------
// 	light = malloc(sizeof(t_light));
// 	if (!light)
// 		return (false);
// 	light->position = (t_vec3){-5, 19, -12};
// 	light->intensity = 0.9;
// 	light->color = (t_color){255, 255, 255};
// 	ft_lstadd_back(&s->lights, add_content(light));

// 	// s->ambient_color = (t_color){255, 255, 255};
// 	// s->ambient_ratio = 0.1;
// 	// s->background_color = (t_color){0, 0, 0};
// 	// return (1);

// 	s->ambient_color = (t_color){255, 255, 255};
// 	s->ambient_ratio = 0.2;
// 	s->background_color = (t_color){10, 10, 40};
// 	return (1);
// }





// // --- FEATURE HIGHLIGHT : reflection + fresnel ---
// // two reflective spheres
// //
// //reflectivity based on refl and ior :
// //	plastic	: shine =  16; spec = 0.3; refl = 0.3; ior = 1.00;
// //	glossy	: shine =  32; spec = 1.0; refl = 1.0; ior = 1.52;
// //	shiny	: shine = 128; spec = 1.0; refl = 1.0; ior = 2.42;
// //	mirror	: shine = 256; spec = 1.0; refl = 1.0; ior = 0.01;
// //	metallic: shine = 256; spec = 1.0; refl = 1.0; ior = 0.18;
// bool	init_scene(t_scene *s)
// {
// 	t_sphere	*sphere;
// 	t_cylinder	*cylinder;
// 	t_plane		*plane;
// 	t_object	*obj;
// 	t_light		*light;

// 	s->camera.position = (t_vec3){3.00, 7.68, -15.15};
// 	s->camera.direction = vec_normalize((t_vec3){-0.05, 0.01, 1.00});
// 	s->camera.fov = 50;

// 	s->is_rendering = false;
// 	s->objects = NULL;
// 	s->lights = NULL;

// 	// ---------- SPHERE ----------
// 	sphere = malloc(sizeof(t_sphere));
// 	if (!sphere)
// 		return (false);
// 	sphere->center = (t_vec3){-10, 10, 30};
// 	sphere->radius = 10;
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_SPHERE;
// 	obj->shape = sphere;
// 	obj->material.base_color = (t_color){255, 60, 60};
// 	obj->material.reflectivity = 1;
// 	obj->material.ior = 1.52;
// 	obj->material.shininess = 32;
// 	obj->material.specular_strength = 1;
// 	obj->material.refractivity = 0;
// 	obj->intersect = intersect_sphere;
// 	obj->get_normal = get_normal_sphere;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	sphere = malloc(sizeof(t_sphere));
// 	if (!sphere)
// 		return (false);
// 	sphere->center = (t_vec3){10, 10, 30};
// 	sphere->radius = 10;
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_SPHERE;
// 	obj->shape = sphere;
// 	obj->material.base_color = (t_color){255, 255, 255};
// 	obj->material.reflectivity = 1;
// 	obj->material.ior = 0.05;
// 	obj->material.shininess = 64;
// 	obj->material.specular_strength = 0.7;
// 	obj->intersect = intersect_sphere;
// 	obj->get_normal = get_normal_sphere;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	// ---------- PLANE ----------
// 	plane = malloc(sizeof(t_plane));
// 	if (!plane)
// 		return (false);
// 	plane->point = (t_vec3){0, 0, 0};
// 	plane->normal = (t_vec3){0, 1, 0};
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_PLANE;
// 	obj->shape = plane;
// 	obj->material.base_color = (t_color){240, 240, 240};
// 	obj->material.reflectivity = 1;
// 	obj->material.ior = 1;
// 	obj->material.specular_strength = 0;
// 	obj->material.shininess = 0;
// 	obj->intersect = intersect_plane;
// 	obj->get_normal = get_normal_plane;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	// ---------- LIGHT ----------
// 	light = malloc(sizeof(t_light));
// 	if (!light)
// 		return (false);
// 	light->position = (t_vec3){0, 30, -30};
// 	light->intensity = 0.9;
// 	light->color = (t_color){255, 255, 255};
// 	ft_lstadd_back(&s->lights, add_content(light));

// 	s->ambient_color = (t_color){255, 255, 255};
// 	s->ambient_ratio = 0.2;
// 	s->background_color = (t_color){10, 10, 40};
// 	return (1);
// }





// //--- FEATURE HIGHLIGHT : specular highlight ---
// //sphere, specular highlight (like in the subject)
// //
// bool	init_scene(t_scene *s)
// {
// 	t_sphere	*sphere;
// 	t_object	*obj;
// 	t_light		*light;

// 	s->camera.position = (t_vec3){0, 2.3, -10};
// 	s->camera.direction = (t_vec3){-0.3, 0, 1};
// 	s->camera.fov = 70;

// 	s->is_rendering = false;
// 	s->objects = NULL;
// 	s->lights = NULL;

// 	// ---------- SPHERE ----------
// 	sphere = malloc(sizeof(t_sphere));
// 	if (!sphere)
// 		return (false);
// 	sphere->center = (t_vec3){-3, 3, 2};
// 	sphere->radius = 3;
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_SPHERE;
// 	obj->shape = sphere;
// 	obj->material.base_color = (t_color){0, 0, 200};
// 	obj->material.reflectivity = 0;
// 	obj->material.shininess = 20;
// 	obj->material.specular_strength = 1.3;
// 	obj->intersect = intersect_sphere;
// 	obj->get_normal = get_normal_sphere;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	// ---------- LIGHT ----------
// 	light = malloc(sizeof(t_light));
// 	if (!light)
// 		return (false);
// 	light->position = (t_vec3){-5, 9, -12};
// 	light->intensity = 0.57;
// 	light->color = (t_color){255, 255, 255};
// 	ft_lstadd_back(&s->lights, add_content(light));

// 	s->ambient_color = (t_color){255, 255, 255};
// 	s->ambient_ratio = 0.01;
// 	s->background_color = (t_color){0, 0, 0};
// 	return (1);
// }





// //--- FEATURE HIGHLIGHT : diffuse + hard shadow stress test---
// //showing initialization for every objects, two planes
//
// bool	init_scene(t_scene *s)
// {
// 	t_sphere	*sphere;
// 	t_sphere	*sphere1;
// 	t_cylinder	*cylinder;
// 	t_cone		*cone;
// 	t_plane		*plane;
// 	t_plane		*plane1;
// 	t_triangle	*tri;
// 	t_object	*obj;
// 	t_light		*light;

// 	(void)plane1;
// 	(void)cylinder;

// 	s->camera.position = (t_vec3){0, 2.3, -10};
// 	s->camera.direction = (t_vec3){-0.3, 0, 1};
// 	s->camera.fov = 70;

// 	s->ambient_color = (t_color){250, 200, 200};
// 	s->ambient_ratio = 0.3;
// 	s->background_color = (t_color){10, 10, 40};
// 	s->is_rendering = false;
// 	s->objects = NULL;
// 	s->lights = NULL;

//	//removing some rendering component bc of uninit material values
//	// g_renderer(NULL)->shading_flag &= ~FLAG_SPECULAR;
// 	// g_renderer(NULL)->shading_flag &= ~FLAG_TRANSPARENT_SHADOW;
// 	g_renderer(NULL)->shading_flag &= ~FLAG_REFLECTION;
// 	g_renderer(NULL)->shading_flag &= ~FLAG_FRESNEL;

// 	// ---------- SPHERE ----------
// 	sphere = malloc(sizeof(t_sphere));
// 	if (!sphere)
// 		return (false);
// 	sphere->center = (t_vec3){-3, 3, 2};
// 	sphere->radius = 3;
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_SPHERE;
// 	obj->shape = sphere;
// 	obj->material.base_color = (t_color){255, 200, 200};
// 	obj->material.reflectivity = 0;
// 	obj->material.shininess = 0;
// 	obj->material.specular_strength = 0;
// 	obj->material.ior = 0.0;
// 	obj->intersect = intersect_sphere;
// 	obj->get_normal = get_normal_sphere;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	sphere1 = malloc(sizeof(t_sphere));
// 	if (!sphere)
// 		return (false);
// 	sphere1->center = (t_vec3){6, 0, 3};
// 	sphere1->radius = 5;

// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_SPHERE;
// 	obj->shape = sphere1;
// 	obj->material.base_color = (t_color){100, 255, 200};
// 	obj->material.reflectivity = 0;
// 	obj->material.shininess = 0;
// 	obj->material.specular_strength = 0;
// 	obj->material.ior = 0.0;
// 	obj->intersect = intersect_sphere;
// 	obj->get_normal = get_normal_sphere;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	// ---------- CYLINDER ----------
// 	cylinder = malloc(sizeof(t_cylinder));
// 	if (!cylinder)
// 		return (false);
// 	cylinder->center = (t_vec3){5, -2, -5};
// 	cylinder->axis = vec_normalize((t_vec3){0.3, 1, -0.2}),
// 	cylinder->radius = 0.5;
// 	cylinder->height = 9;

// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_CYLINDER;
// 	obj->shape = cylinder;
// 	obj->material.base_color = (t_color){100, 100, 200};
// 	obj->material.reflectivity = 0;
// 	obj->material.shininess = 0;
// 	obj->material.specular_strength = 0;
// 	obj->material.ior = 0.0;
// 	obj->intersect = intersect_cylinder;
// 	obj->get_normal = get_normal_cylinder;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	// ---------- CONE ----------
// 	//ANGLE SHOULD be
// 	cone = malloc(sizeof(t_cone));
// 	if (!cone)
// 		return (false);
// 	cone->apex = (t_vec3){-2, 7, -7};
// 	cone->axis = vec_normalize((t_vec3){-0.3, -1, 0.1});
// 	cone->angle = 0.3;
// 	cone->height = 3.5;

// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_CONE;
// 	obj->shape = cone;
// 	obj->material.base_color = (t_color){100, 200, 200};
// 	obj->material.reflectivity = 0;
// 	obj->material.shininess = 0;
// 	obj->material.specular_strength = 0;
// 	obj->material.ior = 0.0;
// 	obj->intersect = intersect_cone;
// 	obj->get_normal = get_normal_cone;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	// ---------- triangle ----------
// 	tri = malloc(sizeof(t_triangle));
// 	if (!tri)
// 		return (false);
// 	tri->p0 = (t_vec3){-14, 13, -1};
// 	tri->p1 = (t_vec3){-12, 0, 2};
// 	tri->p2 = (t_vec3){-9, 1, -9};

// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_TRIANGLE;
// 	obj->shape = tri;
// 	obj->material.base_color = (t_color){255, 200, 200};
// 	obj->material.reflectivity = 0;
// 	obj->material.shininess = 0;
// 	obj->material.specular_strength = 0;
// 	obj->material.ior = 0.0;
// 	obj->intersect = intersect_triangle;
// 	obj->get_normal = get_normal_triangle;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	// ---------- LIGHT ----------
// 	light = malloc(sizeof(t_light));
// 	if (!light)
// 		return (false);
// 	light->position = (t_vec3){-5, 2, -12};
// 	light->intensity = 0.2;
// 	light->color = (t_color){255, 200, 200};
// 	ft_lstadd_back(&s->lights, add_content(light));

// 	light = malloc(sizeof(t_light));
// 	if (!light)
// 		return (false);
// 	light->position = (t_vec3){8, 8, -8};
// 	light->intensity = 0.7;
// 	light->color = (t_color){255, 50, 75};
// 	ft_lstadd_back(&s->lights, add_content(light));

// 	light = malloc(sizeof(t_light));
// 	if (!light)
// 		return (false);
// 	light->position = (t_vec3){-21, 12, -7};
// 	light->intensity = 0.7;
// 	light->color = (t_color){75, 50, 255};
// 	ft_lstadd_back(&s->lights, add_content(light));
	
// 	// ---------- PLANE ----------
// 	plane = malloc(sizeof(t_plane));
// 	if (!plane)
// 		return (false);
// 	plane->point = (t_vec3){0, 0, 0};
// 	plane->normal = (t_vec3){0, 1, 0};
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_PLANE;
// 	obj->shape = plane;
// 	obj->material.base_color = (t_color){255, 255, 255};
// 	obj->material.reflectivity = 0;
// 	obj->material.shininess = 0;
// 	obj->material.specular_strength = 0;
// 	obj->material.ior = 0;
// 	obj->intersect = intersect_plane;
// 	obj->get_normal = get_normal_plane;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	plane1 = malloc(sizeof(t_plane));
// 	if (!plane1)
// 		return (false);
// 	plane1->point = (t_vec3){0, 0, 20};
// 	plane1->normal = (t_vec3){-0.2, 0, -1};
// 	obj = malloc(sizeof(t_object));
// 	if (!obj)
// 		return (false);
// 	obj->type = OBJ_PLANE;
// 	obj->shape = plane1;
// 	obj->material.base_color = (t_color){255, 255, 255};
// 	obj->material.reflectivity = 0;
// 	obj->material.shininess = 0;
// 	obj->material.specular_strength = 0;
// 	obj->material.ior = 0;
// 	obj->intersect = intersect_plane;
// 	obj->get_normal = get_normal_plane;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	return (1);
// }