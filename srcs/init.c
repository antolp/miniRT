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


//hard-coded
//NO OBJECTS CAN BE FREED as of now
//garbage function to test scenes
bool	init_scene(t_scene *s)
{
	t_sphere	*sphere;
	t_plane		*plane;
	t_plane		*plane1;
	t_plane		*plane2;
	t_object	*obj;
	t_light		*light;

	s->camera.position = (t_vec3){0, 2.3, -10};
	s->camera.direction = (t_vec3){-0.3, 0, 1};
	s->camera.fov = 70;

	s->ambient_color = (t_color){250, 200, 200};
	s->ambient_ratio = 0.1f;
	s->background_color = (t_color){0, 0, 0};
	// s->background_color = (t_color){30, 30, 30};
	s->is_rendering = false;
	s->objects = NULL;
	s->lights = NULL;

	// ---------- SPHERE ----------
	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
		return (false);
	sphere->center = (t_vec3){-2, 3, 0};
	sphere->radius = 3;
	sphere->material.base_color = (t_color){255, 255, 255};
	sphere->material.reflectivity = 0;
	sphere->material.shininess = 0;
	sphere->material.specular_coef = 0;
	sphere->material.ior = 1.0;

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_SPHERE;
	obj->shape = sphere;
	obj->material = &sphere->material;
	obj->intersect = intersect_sphere;
	obj->get_normal = get_normal_sphere;
	ft_lstadd_back(&s->objects, add_content(obj));

	// ---------- PLANE ----------
	plane = malloc(sizeof(t_plane));
	if (!plane)
		return (false);
	plane->point = (t_vec3){0, 0, 0};
	plane->normal = (t_vec3){0, 1, 0};
	plane->material.base_color = (t_color){242, 170, 165};

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_PLANE;
	obj->shape = plane;
	obj->material = &plane->material;
	obj->intersect = intersect_plane;
	obj->get_normal = get_normal_plane;
	ft_lstadd_back(&s->objects, add_content(obj));

	plane1 = malloc(sizeof(t_plane));
	if (!plane1)
		return (false);
	plane1->point = (t_vec3){-10, 0, 0};
	plane1->normal = (t_vec3){1, 0, 0};
	plane1->material.base_color = (t_color){20, 20, 50};

	plane2 = malloc(sizeof(t_plane));
	if (!plane1)
		return (false);
	plane2->point = (t_vec3){0, 0, 6};
	plane2->normal = (t_vec3){0, 0, -1};
	plane2->material.base_color = (t_color){255, 225, 225};

	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_PLANE;
	obj->shape = plane2;
	obj->material = &plane2->material;
	obj->intersect = intersect_plane;
	obj->get_normal = get_normal_plane;
	ft_lstadd_back(&s->objects, add_content(obj));
	
	obj = malloc(sizeof(t_object));
	if (!obj)
		return (false);
	obj->type = OBJ_PLANE;
	obj->shape = plane1;
	obj->material = &plane1->material;
	obj->intersect = intersect_plane;
	obj->get_normal = get_normal_plane;
	ft_lstadd_back(&s->objects, add_content(obj));


	// ---------- LIGHT ----------
	light = malloc(sizeof(t_light));
	if (!light)
		return (false);
	light->position = (t_vec3){-5, 2, -5};
	light->intensity = 1;
	light->color = (t_color){255, 50, 80};
	ft_lstadd_back(&s->lights, add_content(light));

	light = malloc(sizeof(t_light));
	if (!light)
		return (false);
	light->position = (t_vec3){8, 8, -8};
	light->intensity = 1.0;
	light->color = (t_color){75, 50, 255};
	ft_lstadd_back(&s->lights, add_content(light));
	return (true);
}
