/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:29:21 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/27 01:50:19 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	handle_debug_keys(int keycode, t_cli_var *var)
{
	if (keycode == XK_slash)
		return (printf("shadign flag : %i\n", var->r->shading_flag), 1);
	if (keycode == XK_b)
	{
		if (var->scale <= 0.1)
			return (printf("Edit scale already minimum."), 1);
		var->scale -= 0.1;
		printf("N pressed, edit scale : %f\n", var->scale);
		var->r->edit.edit_scale = var->scale;
		return (1);
	}
	if (keycode == XK_n)
	{
		var->scale += 0.1;
		printf("N pressed, edit scale : %f\n", var->scale);
		var->r->edit.edit_scale = var->scale;
		return (1);
	}
	if (keycode == XK_p)
		return (print_scene(), 1);
	if (keycode == XK_h)
		return (print_help(), 1);
	return (0);
}

static int	handle_global_keys(int keycode, t_cli_var *var)
{
	if (handle_quit(keycode))
		return (1);
	if (handle_open_flags_menu(keycode, var->r))
		return (1);
	if (handle_edit_menu_keys_wrapper(keycode, var->r))
		return (1);
	if (handle_mode_switch(keycode, var->r))
		return (1);
	if (handle_supersample_toggle(keycode, var->r))
		return (1);
	return (0);
}

static int	handle_edit_mode_keys(int keycode, t_cli_var *var)
{
	if (handle_props_move_ws(keycode, var))
		return (1);
	if (handle_props_move_ad(keycode, var))
		return (1);
	if (handle_props_move_tg(keycode, var))
		return (1);
	if (handle_camera_yaw(keycode, var))
		return (1);
	if (handle_camera_pitch(keycode, var))
		return (1);
	if (handle_camera_fov_in(keycode, var))
		return (1);
	if (handle_camera_fov_out(keycode, var))
		return (1);
	if (handle_debug_keys(keycode, var))
		return (1);
	return (0);
}

static int	init_cli_var(t_cli_var *var, void *param)
{
	(void)param;
	var->r = g_renderer(NULL);
	var->scene = g_scene(NULL);
	if (var->r->is_rendering)
		return (1);
	var->world_up.x = 0.0;
	var->world_up.y = 1.0;
	var->world_up.z = 0.0;
	var->right = vec_normalize(vec_cross(
				var->scene->camera.direction, var->world_up));
	var->scale = var->r->edit.edit_scale;
	return (0);
}

//not using var on should_quit check
//because init_var returns 1 on is_rendering
//(avoids going down key_hook when a render is being computed)
int	key_hook(int keycode, void *param)
{
	t_cli_var	var;
	t_renderer	*r;

	r = g_renderer(NULL);
	if (keycode == XK_Escape)
		r->should_quit = true;
	if (init_cli_var(&var, param))
		return (0);
	if (handle_scene_edit_keys(keycode, &var))
		return (0);
	if (handle_global_keys(keycode, &var))
		return (0);
	if (var.r->mode != RENDER_EDIT)
		return (0);
	if (handle_edit_mode_keys(keycode, &var))
		return (0);
	return (0);
}
