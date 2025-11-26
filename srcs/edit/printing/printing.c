/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:29:55 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:53:35 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static void	print_help1(void)
{
	printf("Scene Editing:\n");
	printf("  scene editing mode :\n");
	printf("  1            : Edit objects\n");
	printf("  2            : Edit lights\n");
	printf("  in scene editing mode :\n");
	printf("  ] / [        : Cycle through prop list\n");
	printf("  WA / SD / TG : move prop (aligned with camera axis)\n");
	printf("  K / ;        : rotate along Y axis (yaw)\n");
	printf("  O / L        : rotate along camera right\n");
	printf("  Z / X (C / V): scale object properties");
	printf("  P            : Print info about current selected prop\n");
	printf("  0            : quit scene editing mode\n\n");
	printf("Misc:\n");
	printf("  Z          : Zoom in  (decrease FOV)\n");
	printf("  X          : Zoom out (increase FOV)\n");
	printf("  B          : Print shading_flag value\n");
	printf("  P          : Print the scene informations\n");
}

void	print_help(void)
{
	printf("=== Controls ===\n");
	printf("Global:\n");
	printf("  E / e      : Switch to EDIT mode (from a rendered frame)\n");
	printf("  R / r      : Switch to HIGH QUALITY RENDER mode");
	printf(" (from EDIT mode)\n");
	printf("  M          : Open the shading menu\n");
	printf("  U          : Toggle SSAA supersampling\n");
	printf("               (Warning !! SSAA highly increases");
	printf(" the rendering time !)\n");
	printf("  ESC        : Quit the program\n\n");
	printf("Camera :\n");
	printf("  camera movement:\n");
	printf("  W / S      : Move camera forward / backward\n");
	printf("  A / D      : Move camera left / right\n");
	printf("  T / G      : Move camera up / down\n");
	printf("  camera rotation:\n");
	printf("  Q / K      : Yaw left\n");
	printf("  E / ;      : Yaw right\n");
	printf("  O          : Pitch up\n");
	printf("  L          : Pitch down\n\n");
	print_help1();
}

//print selected object/lights
void	editor_print_current_object(t_renderer *r)
{
	t_object	*obj;

	if (r->edit.current_obj == NULL)
		return ;
	obj = (t_object *)r->edit.current_obj->content;
	printf("Editing OBJECT:\n");
	if (obj->type == 0)
		printf("coucou\n");
	if (obj->type == 1)
		edit_print_plane(obj);
	if (obj->type == 2)
		edit_print_sphere(obj);
	if (obj->type == 3)
		edit_print_cylinder(obj);
	if (obj->type == 4)
		edit_print_cone(obj);
	if (obj->type == 5)
		edit_print_triangle(obj);
}

void	editor_print_current_light(t_renderer *r)
{
	t_light	*light;

	if (r->edit.current_light == NULL)
		return ;
	light = (t_light *)r->edit.current_light->content;
	printf("Editing LIGHT:\n");
	print_light(light);
}

//general print current object
void	editor_print_current(t_renderer *r)
{
	if (r->edit.target == EDIT_TARGET_OBJECT)
		editor_print_current_object(r);
	else if (r->edit.target == EDIT_TARGET_LIGHT)
		editor_print_current_light(r);
}
