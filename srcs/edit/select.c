/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:29:41 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:47:59 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//next/prev object
static void	editor_next_object(t_renderer *r, t_scene *scene)
{
	if (scene->objects == NULL)
		return ;
	if (r->edit.current_obj == NULL)
		r->edit.current_obj = scene->objects;
	else if (r->edit.current_obj->next != NULL)
		r->edit.current_obj = r->edit.current_obj->next;
	else
		r->edit.current_obj = scene->objects;
	editor_print_current_object(r);
}

static void	editor_prev_object(t_renderer *r, t_scene *scene)
{
	if (scene->objects == NULL)
		return ;
	if (r->edit.current_obj == NULL)
		r->edit.current_obj = scene->objects;
	else
		r->edit.current_obj = editor_list_prev_wrap(
				scene->objects, r->edit.current_obj);
	editor_print_current_object(r);
}

//next/prev light
static void	editor_next_light(t_renderer *r, t_scene *scene)
{
	if (scene->lights == NULL)
		return ;
	if (r->edit.current_light == NULL)
		r->edit.current_light = scene->lights;
	else if (r->edit.current_light->next != NULL)
		r->edit.current_light = r->edit.current_light->next;
	else
		r->edit.current_light = scene->lights;
	editor_print_current_light(r);
}

static void	editor_prev_light(t_renderer *r, t_scene *scene)
{
	if (scene->lights == NULL)
		return ;
	if (r->edit.current_light == NULL)
		r->edit.current_light = scene->lights;
	else
		r->edit.current_light = editor_list_prev_wrap(
				scene->lights, r->edit.current_light);
	editor_print_current_light(r);
}

//cycle through lists to select object
void	editor_cycle_selection(t_renderer *r, t_scene *scene, int keycode)
{
	if (r->edit.target == EDIT_TARGET_OBJECT)
	{
		if (keycode == XK_bracketright)
			editor_next_object(r, scene);
		else if (keycode == XK_bracketleft)
			editor_prev_object(r, scene);
	}
	else if (r->edit.target == EDIT_TARGET_LIGHT)
	{
		if (keycode == XK_bracketright)
			editor_next_light(r, scene);
		else if (keycode == XK_bracketleft)
			editor_prev_light(r, scene);
	}
}
