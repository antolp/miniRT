/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_scene.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 05:17:44 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	print_material(t_material *m)
{
	if (!m)
	{
		printf("No material !\n");
		return ;
	}
	printf("\tshininess :\t%f\n", m->shininess);
	printf("\tspec_strgth :\t%f\n", m->specular_strength);
	printf("\tior :\t\t%f\n", m->ior);
	printf("\treflectivity :\t%f\n", m->reflectivity);
	printf("\trefractivity :\t%f\n", m->refractivity);
	printf("\ttexture :\n");
	print_texture("\t\t", m->texture);
	printf("\tbump maps :\n");
	printf("\t\tbump_strength : %f\n", m->bump_strength);
	print_texture("\t bump:", m->bump_maps);
}

//if we want to push these, we may add the print_(shape)
//as a function pointer to the object struct
void	print_obj(t_object *obj)
{
	if (!obj)
	{
		printf("No obj !\n");
		return ;
	}
	printf("\ntype %i, ", obj->type);
	if (obj->type == 0)
		printf("something is not right....... \n");
	if (obj->type == 1)
		print_plane(obj->shape);
	if (obj->type == 2)
		print_sphere(obj->shape);
	if (obj->type == 3)
		print_cylinder(obj->shape);
	if (obj->type == 4)
		print_cone(obj->shape);
	if (obj->type == 5)
		print_triangle(obj->shape);
	printf("mat :\n");
	print_material(&obj->material);
	printf("\n");
}

void	print_light(t_light *l)
{
	if (!l)
	{
		printf("No light !\n");
		return ;
	}
	print_vec3("\tpos", l->position);
	print_color("\tcolor   ", l->color);
	printf("\tintensity : \t%f\n", l->intensity);
}

void	print_scene1(t_list	*curr, int i)
{
	t_scene	*s;

	s = g_scene(NULL);
	i = 0;
	curr = s->objects;
	printf("--- OBJS ---------------------------\n");
	while (curr != NULL)
	{
		printf(" -- obj %i ----- \n", i++);
		print_obj(curr->content);
		printf(" --------------\n\n");
		curr = curr->next;
	}
	i = 0;
	curr = s->lights;
	printf("--- LIGHTS -------------------------\n");
	while (curr != NULL)
	{
		printf("light %i :\n", i++);
		print_light(curr->content);
		printf("\n");
		curr = curr->next;
	}
	printf("skybox :\n");
	print_texture("\t", s->skybox);
}

void	print_scene(void)
{
	t_scene	*s;
	t_list	*curr;
	int		i;

	curr = NULL;
	i = 0;
	s = g_scene(NULL);
	printf("=== Printing scene ==========================\n");
	if (!s)
	{
		printf("No scene !\n");
		return ;
	}
	printf("camera :\n");
	print_camera(&s->camera);
	print_scene1(curr, i);
	printf("====================================\n");
}
