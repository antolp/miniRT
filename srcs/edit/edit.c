#include "rt.h"

//NEEDS TO BE SPLIT INTO LOTS OF FILES


//two functions to cycle trough lists of obj/lights
t_list	*editor_list_last(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

t_list	*editor_list_prev_wrap(t_list *head, t_list *node)
{
	t_list	*prev;

	if (head == NULL || node == NULL)
		return (NULL);
	if (head == node)
		return (editor_list_last(head));
	prev = head;
	while (prev->next != NULL && prev->next != node)
		prev = prev->next;
	if (prev->next == node)
		return (prev);
	return (node);
}

//print selected object/lights
void	editor_print_current_object(t_renderer *r)
{
	t_object	*obj;

	if (r->edit.current_obj == NULL)
		return ;
	obj = (t_object *)r->edit.current_obj->content;
	printf("Editing OBJECT:\n");
	print_obj(obj);
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


//sets the right target to edit
void	editor_set_target_camera(t_renderer *r)
{
	r->edit.target = EDIT_TARGET_CAMERA;
	printf("Editing CAMERA\n");
}

void	editor_set_target_object(t_renderer *r, t_scene *scene)
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

void	editor_set_target_light(t_renderer *r, t_scene *scene)
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


//next/prev object
void	editor_next_object(t_renderer *r, t_scene *scene)
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

void	editor_prev_object(t_renderer *r, t_scene *scene)
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
void	editor_next_light(t_renderer *r, t_scene *scene)
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

void	editor_prev_light(t_renderer *r, t_scene *scene)
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


//cycle through list to help select object
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

void	editor_print_current(t_renderer *r)
{
	if (r->edit.target == EDIT_TARGET_OBJECT)
		editor_print_current_object(r);
	else if (r->edit.target == EDIT_TARGET_LIGHT)
		editor_print_current_light(r);
}



void	editor_translate_object_shapes(t_object *obj, t_vec3 delta)
{
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
}


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
	editor_translate_object_shapes(obj, delta);
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


void	editor_print_move(t_renderer *r, char *dir)
{
	if (r->edit.target == EDIT_TARGET_CAMERA)
		printf("Move camera %s\n", dir);
	else if (r->edit.target == EDIT_TARGET_OBJECT)
		printf("Move object %s\n", dir);
	else if (r->edit.target == EDIT_TARGET_LIGHT)
		printf("Move light %s\n", dir);
}




//move prop forward backward, whether camera, object or light 
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

//move prop left right, whether camera, object or light 
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
//up down
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


//helper to ignore entry while in scene editing mode
int	is_scene_editing(t_renderer *r)
{
	if (r->edit.target == EDIT_TARGET_OBJECT)
		return (1);
	if (r->edit.target == EDIT_TARGET_LIGHT)
		return (1);
	return (0);
}

//main function for active keys in scene editing mode, 0 goes back to camera mode
int	handle_scene_edit_active_keys(int keycode, t_cli_var *var)
{
	if (keycode == XK_0)
		return (editor_set_target_camera(var->r), 1);
	if (keycode == XK_bracketleft || keycode == XK_bracketright)
		return (editor_cycle_selection(var->r, var->scene, keycode), 1);
	if (keycode == XK_p || keycode == XK_P)
		return (editor_print_current(var->r), 1);
	if (keycode == XK_r || keycode == XK_R)
		return (printf("Close the menu before rendering.\n"), 1);
	if (handle_props_move_ws(keycode, var))
		return (1);
	if (handle_props_move_ad(keycode, var))
		return (1);
	if (handle_props_move_tg(keycode, var))
		return (1);
	return (1);
}

//main scene editing entry point
int	handle_scene_edit_keys(int keycode, t_cli_var *var)
{
	if (var->r->mode != RENDER_EDIT)
		return (0);
	if (keycode == XK_Escape)
		return (0);
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






















double	deg2rad(double angle)
{
	return (angle * M_PI / 180.0);
}
//Rotates around Y axis
t_vec3	rotate_yaw(t_vec3 dir, double angle_deg)
{
	double	angle;
	double	cos_a;
	double	sin_a;
	t_vec3	new_dir;

	angle = deg2rad(angle_deg);
	cos_a = cos(angle);
	sin_a = sin(angle);
	new_dir.x = dir.x * cos_a + dir.z * sin_a;
	new_dir.y = dir.y;
	new_dir.z = -dir.x * sin_a + dir.z * cos_a;
	return (vec_normalize(new_dir));
}

//rotates around right vector using Rodrigues' rotation formula
static t_vec3	rotate_pitch_rodrigues(t_vec3 dir, t_vec3 right,
			double cos_a, double sin_a)
{
	double	dot_rd;
	t_vec3	cross_rd;
	t_vec3	new_dir;

	dot_rd = vec_dot(right, dir);
	cross_rd = vec_cross(right, dir);
	new_dir.x = dir.x * cos_a + right.x * dot_rd * (1.0 - cos_a)
		+ cross_rd.x * sin_a;
	new_dir.y = dir.y * cos_a + right.y * dot_rd * (1.0 - cos_a)
		+ cross_rd.y * sin_a;
	new_dir.z = dir.z * cos_a + right.z * dot_rd * (1.0 - cos_a)
		+ cross_rd.z * sin_a;
	return (vec_normalize(new_dir));
}

t_vec3	rotate_pitch(t_vec3 dir, double angle_deg)
{
	double	angle;
	double	cos_a;
	double	sin_a;
	t_vec3	world_up;
	t_vec3	right;

	angle = deg2rad(angle_deg);
	cos_a = cos(angle);
	sin_a = sin(angle);
	world_up = (t_vec3){0.0, 1.0, 0.0};
	right = vec_normalize(vec_cross(dir, world_up));
	return (rotate_pitch_rodrigues(dir, right, cos_a, sin_a));
}

static int	handle_quit(int keycode)
{
	if (keycode == XK_Escape)
	{
		printf("Exiting...\n");
		exit(0);
	}
	return (0);
}

static int	handle_mode_switch(int keycode, t_renderer *r)
{
	if ((keycode == XK_E || keycode == XK_e)
		&& r->mode != RENDER_EDIT)
	{
		r->mode = RENDER_EDIT;
		r->has_drawn_realtime = false;
		printf("Switched to EDIT mode\nPress H for help\n");
		return (1);
	}
	if ((keycode == XK_R || keycode == XK_r)
		&& r->mode != RENDER_HIGH_QUALITY)
	{
		if (r->mode != RENDER_EDIT_MENU)
		{
			r->mode = RENDER_HIGH_QUALITY;
			r->render_done = false;
			printf("Switched to RENDER mode\n");
		}
		return (1);
	}
	return (0);
}

static int	handle_supersample_toggle(int keycode, t_renderer *r)
{
	if (keycode == XK_u)
	{
		if (r->supersampled == false)
		{
			printf("supersample on.\n");
			r->supersampled = true;
		}
		else
		{
			printf("supersample off.\n");
			r->supersampled = false;
		}
		return (1);
	}
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

static int	handle_camera_yaw(int keycode, t_cli_var *var)
{
	if (keycode == XK_q || keycode == XK_k)
	{
		printf("Q pressed: Rotate camera left (yaw)\n");
		var->scene->camera.direction = rotate_yaw(
				var->scene->camera.direction, -var->scale);
		var->r->has_drawn_realtime = false;
		return (1);
	}
	if (keycode == XK_e || keycode == XK_semicolon)
	{
		printf("E pressed: Rotate camera right (yaw)\n");
		var->scene->camera.direction = rotate_yaw(
				var->scene->camera.direction, var->scale);
		var->r->has_drawn_realtime = false;
		return (1);
	}
	return (0);
}

static int	handle_camera_pitch(int keycode, t_cli_var *var)
{
	if (keycode == XK_o)
	{
		printf("O pressed: Rotate camera upward (pitch)\n");
		var->scene->camera.direction = rotate_pitch(
				var->scene->camera.direction, var->scale);
		var->r->has_drawn_realtime = false;
		return (1);
	}
	if (keycode == XK_l)
	{
		printf("L pressed: Rotate camera downward (pitch)\n");
		var->scene->camera.direction = rotate_pitch(
				var->scene->camera.direction, -var->scale);
		var->r->has_drawn_realtime = false;
		return (1);
	}
	return (0);
}

static int	handle_camera_fov_in(int keycode, t_cli_var *var)
{
	if (keycode == XK_z && var->scene->camera.fov > 10)
	{
		printf("Z pressed: Zoom in (fov : %f)\n",
			var->scene->camera.fov - 5);
		if (var->scene->camera.fov > 10)
			var->scene->camera.fov -= 5;
		else
			printf("Minimum FOV reached\n");
		var->r->has_drawn_realtime = false;
		return (1);
	}
	return (0);
}

static int	handle_camera_fov_out(int keycode, t_cli_var *var)
{
	if (keycode == XK_x && var->scene->camera.fov < 130)
	{
		printf("X pressed: Zoom out (fov : %f)\n",
			var->scene->camera.fov + 5);
		if (var->scene->camera.fov < 130)
			var->scene->camera.fov += 5;
		else
			printf("Maximum FOV reached\n");
		var->r->has_drawn_realtime = false;
		return (1);
	}
	return (0);
}

static int	handle_edit_supersample_u(int keycode, t_cli_var *var)
{
	if (keycode == XK_u)
	{
		if (var->r->supersampled == false)
		{
			printf("supersample on.\n");
			var->r->supersampled = true;
		}
		else if (var->r->supersampled == true)
		{
			printf("supersample off.\n");
			var->r->supersampled = false;
		}
		return (1);
	}
	return (0);
}

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
	if (handle_edit_supersample_u(keycode, var))
		return (1);
	if (handle_debug_keys(keycode, var->r))
		return (1);
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
