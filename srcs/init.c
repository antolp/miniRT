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

// //studying shadows on a simple scene
bool	init_scene(t_scene *s)
{
	t_sphere	*sphere;
	t_triangle	*tri;
	t_plane		*plane;
	t_object	*obj;
	t_light		*light;

	s->camera.position = (t_vec3){15, 5, -20};
	s->camera.direction = (t_vec3){-1, -0.2, 0};
	s->camera.fov = 70;

	// s->ambient_color = (t_color){250, 200, 200};
	s->ambient_color = (t_color){0, 0, 0};
	s->ambient_ratio = 0;
	s->background_color = (t_color){0, 0, 0};
	s->is_rendering = false;
	s->objects = NULL;
	s->lights = NULL;

	// ---------- triangle ----------
	tri = malloc(sizeof(t_triangle));
	if (!tri)
		return (false);
	tri->p0 = (t_vec3){-14, 13, -1};
	tri->p1 = (t_vec3){-12, 0, 2};
	tri->p2 = (t_vec3){-9, 1, -9};
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_TRIANGLE;
	obj->shape = tri;
	obj->material.base_color = (t_color){255, 200, 200};
	obj->material.reflectivity = 0;
	obj->material.shininess = 0;
	obj->material.specular_coef = 0;
	obj->material.ior = 1.0;
	obj->intersect = intersect_triangle;
	obj->get_normal = get_normal_triangle;
	ft_lstadd_back(&s->objects, add_content(obj));

	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return (false);
	sphere->center = (t_vec3){0, 1, 0};
	sphere->radius = 1;
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_SPHERE;
	obj->shape = sphere;
	obj->material.base_color = (t_color){255, 0, 0};
	obj->material.reflectivity = 0;
	obj->material.shininess = 0;
	obj->material.specular_coef = 0;
	obj->material.ior = 1.0;
	obj->intersect = intersect_sphere;
	obj->get_normal = get_normal_sphere;
	ft_lstadd_back(&s->objects, add_content(obj));

	// ---------- LIGHT ----------
	light = malloc(sizeof(t_light));
	if (!light)
		return (false);
	light->position = (t_vec3){15, 20, -1};
	light->intensity = 0.9;
	light->color = (t_color){255, 255, 255};
	ft_lstadd_back(&s->lights, add_content(light));

	// light = malloc(sizeof(t_light));
	// if (!light)
	// 	return (false);
	// light->position = (t_vec3){3, 20, 20};
	// light->intensity = 0.8;
	// light->color = (t_color){255, 255, 255};
	// ft_lstadd_back(&s->lights, add_content(light));
	
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
	obj->intersect = intersect_plane;
	obj->get_normal = get_normal_plane;
	ft_lstadd_back(&s->objects, add_content(obj));

	return (1);
}


// // every objects, two planes
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
// 	obj->material.specular_coef = 0;
// 	obj->material.ior = 1.0;
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
// 	obj->material.specular_coef = 0;
// 	obj->material.ior = 1.0;
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
// 	obj->material.specular_coef = 0;
// 	obj->material.ior = 1.0;
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
// 	obj->material.specular_coef = 0;
// 	obj->material.ior = 1.0;obj->intersect = intersect_cone;
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
// 	obj->material.specular_coef = 0;
// 	obj->material.ior = 1.0;
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
// 	obj->intersect = intersect_plane;
// 	obj->get_normal = get_normal_plane;
// 	ft_lstadd_back(&s->objects, add_content(obj));

// 	return (1);
// }