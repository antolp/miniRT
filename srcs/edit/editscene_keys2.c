#include "rt.h"

int	handle_props(int keycode, t_cli_var *var)
{
	if (handle_props_move_ws(keycode, var))
		return (1);
	if (handle_props_move_ad(keycode, var))
		return (1);
	if (handle_props_move_tg(keycode, var))
		return (1);
	if (handle_props_rotate_yaw(keycode, var))
		return (1);
	if (handle_props_rotate_pitch(keycode, var))
		return (1);
	if (handle_props_scale_primary(keycode, var))
		return (1);
	if (handle_props_scale_secondary(keycode, var))
		return (1);
	return (1);
}

//main function for active keys in scene editing mode, 0 goes back to camera mode
int	handle_scene_edit_active_keys(int keycode, t_cli_var *var)
{
	if (keycode == XK_0)
	{
		var->r->edit.target = EDIT_TARGET_CAMERA;
		printf("Back to Camera mode\n");
		return (1);
	}
	if (keycode == XK_bracketleft || keycode == XK_bracketright)
	{
		editor_cycle_selection(var->r, var->scene, keycode);
		return (1);
	}
	if (keycode == XK_p || keycode == XK_P)
	{
		if (var->r->edit.target == EDIT_TARGET_OBJECT)
			print_obj((t_object *)var->r->edit.current_obj->content);
		else if (var->r->edit.target == EDIT_TARGET_OBJECT)
			editor_print_current_light(var->r);
		return (1);
	}
	if (keycode == XK_r || keycode == XK_R)
	{
		printf("Close the menu before rendering.\n");
		return (1);
	}
	return (handle_props(keycode, var));
}
