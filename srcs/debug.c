/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	print_help1(void)
{
	printf("enter scene editing:\n");
	printf("  1          : Edit objects\n");
	printf("  2          : Edit lights\n");
	printf("in scene editing mode :\n");
	printf("  WASD/TG    : move prop (aligned with camera axis)\n");
	printf("  ] / [      : Cycle through prop list\n");
	printf("  P          : Print info about current selected prop\n");
	printf("  0          : quit scene editing mode\n\n");
	printf("Misc:\n");
	printf("  Z          : Zoom in  (decrease FOV)\n");
	printf("  X          : Zoom out (increase FOV)\n\n");
	printf("  B          : Print shading_flag value\n");
	printf("  P          : Print the scene informations\n");
}

void	print_help(void)
{
	printf("=== Controls ===\n");
	printf("Global:\n");
	printf("  E / e      : Switch to EDIT mode (from a rendered frame)\n");
	printf("  R / r      : Switch to HIGH QUALITY RENDER mode (from EDIT mode)\n");
	printf("  M          : Open the shading menu");
	printf("  U          : Toggle SSAA supersampling ");
	printf("(Warning !! this highly increases the rendering time !)\n");
	printf("  ESC        : Quit the program\n\n");
	printf("camera movement:\n");
	printf("  W / S      : Move camera forward / backward\n");
	printf("  A / D      : Move camera left / right\n");
	printf("  T / G      : Move camera up / down\n\n");
	printf("camera rotation:\n");
	printf("  Q / K      : Yaw left\n");
	printf("  E / ;      : Yaw right\n");
	printf("  O          : Pitch up\n");
	printf("  L          : Pitch down\n\n");
	print_help1();
}

void	print_vec3(char *s, t_vec3 v)
{
	if (ft_strlen(s) > 5)
		printf("%s :\t(%.2f, %.2f, %.2f)\n", s, v.x, v.y, v.z);
	else
		printf("%s :\t\t(%.2f, %.2f, %.2f)\n", s, v.x, v.y, v.z);
}

void	print_color(char *s, t_color c)
{
	if (ft_strlen(s) > 5)
		ft_printf("%s :\t(R: %d, G: %d, B: %d)\n", s, c.r, c.g, c.b);
	else
		ft_printf("%s :\t\t(R: %d, G: %d, B: %d)\n", s, c.r, c.g, c.b);

}

void	print_camera(t_camera *c)
{
	if (!c)
		printf("No camera !\n");
	printf("--- CAMERA -------------------------\n");
	print_vec3("pos", c->position);
	print_vec3("dir", c->direction);
	printf("fov :\t\t%.2f deg\n", c->fov);
	printf("------------------------------------\n");
}

void	print_sphere(t_sphere *s)
{
	printf("sphere\n");
	if (!s)
	{
		printf("No sphere !\n");
		return ;
	}
	print_vec3("\tcenter", s->center);
	printf("\tradius : \t%f\n", s->radius);
}

void	print_plane(t_plane *p)
{
	printf("plane\n");
	if (!p)
	{
		printf("No plane !\n");
		return ;
	}
	print_vec3("\tpoint ", p->point);
	print_vec3("\tnormal", p->normal);
}

void	print_cylinder(t_cylinder *cyl)
{
	printf("cylinder\n");
	if (!cyl)
	{
		printf("No cylinder !\n");
		return ;
	}
	print_vec3("\tcenter", cyl->center);
	print_vec3("\taxis", cyl->axis);
	printf("\tradius :\t%f\n", cyl->radius);
	printf("\theight :\t%f\n", cyl->height);
}

void	print_cone(t_cone *co)
{
	printf("cone\n");
	if (!co)
	{
		printf("No cone !\n");
		return ;
	}
	print_vec3("\tapex", co->apex);
	print_vec3("\taxis", co->axis);
	printf("\tangle : \t%f\n", co->angle);
	printf("\theight :\t%f\n", co->height);
}

void	print_triangle(t_triangle *t)
{
	printf("triangle\n");
	if (!t)
	{
		printf("No triangle !\n");
		return ;
	}
	print_vec3("\tp0", t->p0);
	print_vec3("\tp1", t->p1);
	print_vec3("\tp2", t->p2);
	printf("\tuv mode : %i", t->uv_mode);
	printf("\n");
}

void	print_texture(char *s, t_texture	tex)
{
	printf("%stype : %i\n", s, tex.type);
	printf("%stex pl addr : %p\n", s, tex.data);
}

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
	printf("\t\tbump map pl addr: %p\n", m->texture.bumps_data);
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
	t_scene *s = g_scene(NULL);

	i = 0;
	curr = s->objects;
	printf("--- OBJS ---------------------------\n");
	while(curr != NULL)
	{
		printf(" -- obj %i ----- \n", i++);
		print_obj(curr->content);
		printf(" --------------\n");
		printf("\n");
		curr = curr->next;
	}
	i = 0;
	curr = s->lights;
	printf("--- LIGHTS -------------------------\n");
	while(curr != NULL)
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
	t_scene *s = g_scene(NULL);
	t_list	*curr;
	int		i;
	
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
