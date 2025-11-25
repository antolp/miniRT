#include "rt.h"

//camera
void	editor_translate_camera(t_cli_var *var, t_vec3 delta)
{
	var->scene->camera.position = vec_add(
			var->scene->camera.position, delta);
	var->r->has_drawn_realtime = false;
}

//obj
void	editor_translate_object(t_cli_var *var, t_vec3 delta)
{
	t_object	*obj;

	if (var->r->edit.current_obj == NULL)
		return ;
	obj = (t_object *)var->r->edit.current_obj->content;
	if (obj->type == OBJ_SPHERE)
		editor_translate_sphere((t_sphere *)obj->shape, delta);
	else if (obj->type == OBJ_PLANE)
		editor_translate_plane((t_plane *)obj->shape, delta);
	else if (obj->type == OBJ_CYLINDER)
		editor_translate_cylinder((t_cylinder *)obj->shape, delta);
	else if (obj->type == OBJ_CONE)
		editor_translate_cone((t_cone *)obj->shape, delta);
	else if (obj->type == OBJ_TRIANGLE)
		editor_translate_triangle((t_triangle *)obj->shape, delta);
	var->r->has_drawn_realtime = false;
}

//light
void	editor_translate_light(t_cli_var *var, t_vec3 delta)
{
	t_light	*light;

	if (var->r->edit.current_light == NULL)
		return ;
	light = (t_light *)var->r->edit.current_light->content;
	light->position = vec_add(light->position, delta);
	var->r->has_drawn_realtime = false;
}


//main translation entry point
void	editor_translate(t_cli_var *var, t_vec3 delta)
{
	if (var->r->edit.target == EDIT_TARGET_CAMERA)
		editor_translate_camera(var, delta);
	else if (var->r->edit.target == EDIT_TARGET_OBJECT)
		editor_translate_object(var, delta);
	else if (var->r->edit.target == EDIT_TARGET_LIGHT)
		editor_translate_light(var, delta);
}
