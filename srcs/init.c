/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/27 20:03:23 by anle-pag         ###   ########.fr       */
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
	r->should_quit = 0;
	r->is_rendering = 0;
	r->render_done = 0;
	r->supersampled = false;
	r->shading_flag = DEFAULT_FLAGS;
	r->edit.target = EDIT_TARGET_CAMERA;
	r->edit.current_obj = NULL;
	r->edit.current_light = NULL;
	r->edit.edit_scale = 2.5;
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
// t_plane			*plane;
// t_sphere		*sphere;
// t_cylinder		*cyl;
// t_cone			*co;
// t_object		*obj;
// t_light			*light;
// t_triangle		*tri;
// sphere = malloc(sizeof(t_sphere));
// if (!sphere)
// 	return (false);
// sphere->center = (t_vec3){-1.50, 3.00, 1.50};
// sphere->radius = 2.0;
// obj = malloc(sizeof(t_object));
// if (!obj)
// 	return (false);
// obj->type = OBJ_SPHERE;
// obj->shape = sphere;
// obj->material.base_color = (t_color){255,255,255};
// obj->material.reflectivity = 1;
// obj->material.ior = 0.01;
// obj->material.shininess = 64;
// obj->material.specular_strength = 0.9;
// obj->material.refractivity = 0;
// obj->material.texture.type = TEXTURE_NONE;
// obj->material.texture.data = NULL;
// obj->material.bump_maps.type = TEXTURE_NONE;
// obj->material.bump_maps.data = NULL;
// obj->material.bump_strength = 1;
// obj->intersect = intersect_sphere;
// obj->get_normal = get_normal_sphere;
// obj->get_uv = get_uv_sphere;
// ft_lstadd_back(&s->objects, add_content(obj));
bool	init_scene(t_scene *s)
{
	s->ambient_color = (t_color){255,200,200};
	s->ambient_ratio = 0.15;
	return (1);
}
