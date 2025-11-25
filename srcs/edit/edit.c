#include "rt.h"

static int	handle_debug_keys(int keycode, t_renderer *r)
{
	if (keycode == XK_b)
	{
		printf("flag : %i\n", r->shading_flag);
		return (1);
	}
	if (keycode == XK_p)
	{
		print_scene();
		return (1);
	}
	if (keycode == XK_h)
	{
		print_help();
		return (1);
	}
	return (0);
}

static int	handle_global_keys(int keycode, t_cli_var *var)
{
	if (handle_quit(keycode))
		return (1);
	if (handle_edit_menu_keys_wrapper(keycode, var->r))
		return (1);
	if (handle_mode_switch(keycode, var->r))
		return (1);
	if (handle_open_flags_menu(keycode, var->r))
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
	if (handle_debug_keys(keycode, var->r))
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
	var->scale = 3.0;
	return (0);
}

int	key_hook(int keycode, void *param)
{
	t_cli_var	var;

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
