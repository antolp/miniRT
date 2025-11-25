#include "rt.h"

int	handle_props_rotate_yaw(int keycode, t_cli_var *var)
{
	double	angle;

	if (keycode != XK_k && keycode != XK_K
		&& keycode != XK_semicolon)
		return (0);
	if (var->r->edit.target == EDIT_TARGET_LIGHT)
	{
		printf("Lights cannot be rotated.\n");
		return (1);
	}
	if (var->r->edit.target != EDIT_TARGET_OBJECT)
		return (1);
	angle = var->scale;
	if (keycode == XK_k || keycode == XK_K)
		angle = -var->scale;
	editor_rotate_object_yaw(var, angle);
	return (1);
}

int	handle_props_rotate_pitch(int keycode, t_cli_var *var)
{
	double	angle;

	if (keycode != XK_o && keycode != XK_O
		&& keycode != XK_l && keycode != XK_L)
		return (0);
	if (var->r->edit.target == EDIT_TARGET_LIGHT)
	{
		printf("Lights cannot be rotated.\n");
		return (1);
	}
	if (var->r->edit.target != EDIT_TARGET_OBJECT)
		return (1);
	angle = var->scale;
	if (keycode == XK_o || keycode == XK_O)
		angle = var->scale;
	else
		angle = -var->scale;
	editor_rotate_object_pitch(var, angle);
	return (1);
}


//helper for triangle rotation
t_vec3	editor_triangle_center(t_triangle *tri)
{
	t_vec3	center;

	center = vec_add(tri->p0, tri->p1);
	center = vec_add(center, tri->p2);
	center = vec_mul(center, 1.0 / 3.0);
	return (center);
}