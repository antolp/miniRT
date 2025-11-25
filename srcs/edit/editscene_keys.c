#include "rt.h"

//helper for norm
static int	is_scene_editing(t_renderer *r)
{
	if (r->edit.target == EDIT_TARGET_OBJECT)
		return (1);
	if (r->edit.target == EDIT_TARGET_LIGHT)
		return (1);
	return (0);
}

static void	editor_set_target_object(t_renderer *r, t_scene *scene)
{
	if (scene->objects == NULL)
	{
		printf("No objects to edit.\n");
		r->edit.target = EDIT_TARGET_CAMERA;
		return ;
	}
	r->edit.target = EDIT_TARGET_OBJECT;
	if (r->edit.current_obj == NULL)
		r->edit.current_obj = scene->objects;
	editor_print_current_object(r);
}

static void	editor_set_target_light(t_renderer *r, t_scene *scene)
{
	if (scene->lights == NULL)
	{
		printf("No lights to edit.\n");
		r->edit.target = EDIT_TARGET_CAMERA;
		return ;
	}
	r->edit.target = EDIT_TARGET_LIGHT;
	if (r->edit.current_light == NULL)
		r->edit.current_light = scene->lights;
	editor_print_current_light(r);
}

//main scene editing entry point
int	handle_scene_edit_keys(int keycode, t_cli_var *var)
{
	if (var->r->mode != RENDER_EDIT)
		return (0);
	if (keycode == XK_Escape)
	{
		var->r->should_quit = true;
		return (0);
	}
	if (keycode == XK_1)
	{
		editor_set_target_object(var->r, var->scene);
		return (1);
	}
	if (keycode == XK_2)
	{
		editor_set_target_light(var->r, var->scene);
		return (1);
	}
	if (!is_scene_editing(var->r))
		return (0);
	return (handle_scene_edit_active_keys(keycode, var));
}
