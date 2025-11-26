/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   props_move.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:30:14 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:52:15 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	editor_print_move(t_renderer *r, char *dir)
{
	if (r->edit.target == EDIT_TARGET_CAMERA)
		printf("Move camera %s\n", dir);
	else if (r->edit.target == EDIT_TARGET_OBJECT)
		printf("Move object %s\n", dir);
	else if (r->edit.target == EDIT_TARGET_LIGHT)
		printf("Move light %s\n", dir);
}

//props can be cameras, objects or lights
//function here print the accurate message
//calculate the right delta 
//and then call the general translatation function
//
//move prop forward backward
int	handle_props_move_ws(int keycode, t_cli_var *var)
{
	t_vec3	delta;

	if (keycode == XK_w)
	{
		editor_print_move(var->r, "forward");
		delta = vec_mul(var->scene->camera.direction, var->scale);
		editor_translate(var, delta);
		return (1);
	}
	if (keycode == XK_s)
	{
		editor_print_move(var->r, "backward");
		delta = vec_mul(var->scene->camera.direction, -var->scale);
		editor_translate(var, delta);
		return (1);
	}
	return (0);
}

//move prop left right
int	handle_props_move_ad(int keycode, t_cli_var *var)
{
	t_vec3	delta;

	if (keycode == XK_a)
	{
		editor_print_move(var->r, "left");
		delta = vec_mul(var->right, var->scale * 0.5);
		editor_translate(var, delta);
		return (1);
	}
	if (keycode == XK_d)
	{
		editor_print_move(var->r, "right");
		delta = vec_mul(var->right, -var->scale * 0.5);
		editor_translate(var, delta);
		return (1);
	}
	return (0);
}

//move prop up down
int	handle_props_move_tg(int keycode, t_cli_var *var)
{
	t_vec3	delta;

	if (keycode == XK_t)
	{
		editor_print_move(var->r, "up");
		delta = vec_mul(var->world_up, var->scale * 0.075);
		editor_translate(var, delta);
		return (1);
	}
	if (keycode == XK_g)
	{
		editor_print_move(var->r, "down");
		delta = vec_mul(var->world_up, -var->scale * 0.075);
		editor_translate(var, delta);
		return (1);
	}
	return (0);
}
