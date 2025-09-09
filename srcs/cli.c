#include "rt.h"
#define DEG2RAD(angle) ((angle) * M_PI / 180.0)


//Rotates around Y axis
t_vec3 rotate_yaw(t_vec3 dir, double angle_deg)
{
	double angle = DEG2RAD(angle_deg);
	double cos_a = cos(angle);
	double sin_a = sin(angle);

	t_vec3 new_dir;
	new_dir.x = dir.x * cos_a + dir.z * sin_a;
	new_dir.y = dir.y;
	new_dir.z = -dir.x * sin_a + dir.z * cos_a;
	return vec_normalize(new_dir);
}

//rotates around right vector using Rodrigues' rotation formula
t_vec3 rotate_pitch(t_vec3 dir, double angle_deg)
{
	double angle = DEG2RAD(angle_deg);
	double cos_a = cos(angle);
	double sin_a = sin(angle);

	t_vec3 world_up = {0, 1, 0};
	t_vec3 right = vec_normalize(vec_cross(dir, world_up));

	t_vec3 new_dir;
	new_dir.x = dir.x * cos_a + right.x * (vec_dot(right, dir)) * (1 - cos_a) + vec_cross(right, dir).x * sin_a;
	new_dir.y = dir.y * cos_a + right.y * (vec_dot(right, dir)) * (1 - cos_a) + vec_cross(right, dir).y * sin_a;
	new_dir.z = dir.z * cos_a + right.z * (vec_dot(right, dir)) * (1 - cos_a) + vec_cross(right, dir).z * sin_a;

	return vec_normalize(new_dir);
}



int	key_hook(int keycode, void *param)
{
	t_renderer	*r = g_renderer(NULL);
	t_scene		*scene = g_scene(NULL);
	const t_vec3 world_up = {0, 1, 0};
	t_vec3 right = vec_normalize(vec_cross(scene->camera.direction, world_up));
	double scale = 3.0;

	(void)param;
	if (r->is_rendering)
		return (0);

	// === MODE SWITCHING ===
	if ((keycode == XK_E || keycode == XK_e) && r->mode != RENDER_REALTIME)
	{
		r->mode = RENDER_REALTIME;
		r->has_drawn_realtime = false;
		printf("Switched to REALTIME mode (edit)\n");
		return (0);
	}
	else if ((keycode == XK_R || keycode == XK_r) && r->mode != RENDER_HIGH_QUALITY)
	{
		r->mode = RENDER_HIGH_QUALITY;
		r->render_done = false;
		printf("Switched to RENDER mode\n");
		return (0);
	}

	// === EXIT ===
	if (keycode == XK_Escape)
	{
		printf("Exiting...\n");
		// free resources if needed
		exit(0);
	}

	// === EDIT MODE CAMERA CONTROLS ===
	if (r->mode != RENDER_REALTIME)
		return (0);

	if (keycode == XK_w)
	{
		printf("W pressed: Move camera forward\n");
		scene->camera.position = vec_add(scene->camera.position, vec_mul(scene->camera.direction, scale));
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_s)
	{
		printf("S pressed: Move camera backward\n");
		scene->camera.position = vec_sub(scene->camera.position, vec_mul(scene->camera.direction, scale));
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_a)
	{
		printf("A pressed: Move camera left\n");
		scene->camera.position = vec_add(scene->camera.position, vec_mul(right, scale * 0.5));
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_d)
	{
		printf("D pressed: Move camera right\n");
		scene->camera.position = vec_sub(scene->camera.position, vec_mul(right, scale * 0.5));
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_t)
	{
		printf("T pressed: Move camera up\n");
		scene->camera.position = vec_add(scene->camera.position, vec_mul(world_up, scale * 0.5));
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_g)
	{
		printf("G pressed: Move camera down\n");
		scene->camera.position = vec_sub(scene->camera.position, vec_mul(world_up, scale * 0.5));
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_q || keycode == XK_k)
	{
		printf("Q pressed: Rotate camera left (yaw)\n");
		scene->camera.direction = rotate_yaw(scene->camera.direction, -scale);
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_e || keycode == XK_semicolon)
	{
		printf("E pressed: Rotate camera right (yaw)\n");
		scene->camera.direction = rotate_yaw(scene->camera.direction, scale);
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_o)
	{
		printf("O pressed: Rotate camera upward (pitch)\n");
		scene->camera.direction = rotate_pitch(scene->camera.direction, scale);
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_l)
	{
		printf("L pressed: Rotate camera downward (pitch)\n");
		scene->camera.direction = rotate_pitch(scene->camera.direction, -scale);
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_z && scene->camera.fov > 10)
	{
		printf("Z pressed: Zoom in (fov : %f)\n", scene->camera.fov - 5);
		if (scene->camera.fov > 10)
			scene->camera.fov -= 5;
		else
			printf("Minimum FOV reached\n");
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_x && scene->camera.fov < 130)
	{
		printf("X pressed: Zoom out (fov : %f)\n", scene->camera.fov + 5);
		if (scene->camera.fov < 130)
			scene->camera.fov += 5;
		else
			printf("Maximum FOV reached\n");
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_m)
	{
		if (r->supersampled == false)
		{
			printf("supersample on.\n");
			r->supersampled = true;
		}
		else if (r->supersampled == true)
		{
			printf("supersample off.\n");
			r->supersampled = false;
		}
	}
	else if (keycode == XK_n)
	{
		if ((r->shading_flag & FLAG_FRESNEL) != 0u)
		{
			printf("fresnel off.\n");
			r->shading_flag &= ~FLAG_FRESNEL;
		}
		else
		{
			printf("fresnel on.\n");
			r->shading_flag |= FLAG_FRESNEL;
		}
	}

	return (0);
}



// // PROVISOIRE
// int	key_hook(int keycode, void *param)
// {
// 	t_renderer	*r;
// 	t_scene		*scene;

// 	r = g_renderer(NULL);
// 	scene = g_scene(NULL);
// 	const t_vec3 world_up = {0, 1, 0};
// 	double scale;

//     t_vec3 right;
// 	scale = 3.0;
//     right = vec_normalize(vec_cross(scene->camera.direction, world_up));
// 	(void)param;

// 	if (r->is_rendering)
// 		return (0);
// 	if (keycode == XK_Escape)
// 	{
// 		printf("Exiting...\n");
// 		//some free() function 
// 		exit(0);
// 	}
// 	else if (keycode == XK_w)
// 	{
// 		printf("W pressed: Move camera forward\n");
// 		scene->camera.position.x += scene->camera.direction.x * scale;
// 		scene->camera.position.y += scene->camera.direction.y * scale;
// 		scene->camera.position.z += scene->camera.direction.z * scale;
// 	}
// 	else if (keycode == XK_s)
// 	{
// 		printf("S pressed: Move camera backward\n");
// 		scene->camera.position.x -= scene->camera.direction.x * scale;
// 		scene->camera.position.y -= scene->camera.direction.y * scale;
// 		scene->camera.position.z -= scene->camera.direction.z * scale;
// 	}
// 	else if (keycode == XK_a)
// 	{
// 		printf("A pressed: Move camera left\n");
// 		scene->camera.position.x += right.x * scale * 0.5;
// 		scene->camera.position.y += right.y * scale * 0.5;
// 		scene->camera.position.z += right.z * scale * 0.5;
// 	}
// 	else if (keycode == XK_d)
// 	{
// 		printf("D pressed: Move camera right\n");
// 		scene->camera.position.x -= right.x * scale * 0.5;
// 		scene->camera.position.y -= right.y * scale * 0.5;
// 		scene->camera.position.z -= right.z * scale * 0.5;
// 	}
// 	else if (keycode == XK_t)
// 	{
// 		printf("T pressed: Move camera up\n");
// 		scene->camera.position.x += world_up.x * scale * 0.5;
// 		scene->camera.position.y += world_up.y * scale * 0.5;
// 		scene->camera.position.z += world_up.z * scale * 0.5;
// 	}
// 	else if (keycode == XK_g)
// 	{
// 		printf("G pressed: Move camera down\n");
// 		scene->camera.position.x -= world_up.x * scale * 0.5;
// 		scene->camera.position.y -= world_up.y * scale * 0.5;
// 		scene->camera.position.z -= world_up.z * scale * 0.5;
// 	}
// 	else if (keycode == XK_q || keycode == XK_k)
// 	{
// 		printf("Q or K pressed: Rotate camera left (yaw)\n");
// 		scene->camera.direction = rotate_yaw(scene->camera.direction, -scale); // negative for left
// 	}
// 	else if (keycode == XK_e || keycode == XK_semicolon)
// 	{
// 		printf("E or ; pressed: Rotate camera right (yaw)\n");
// 		scene->camera.direction = rotate_yaw(scene->camera.direction, scale);
// 	}
// 	else if (keycode == XK_o)
// 	{
// 		printf("O pressed: Rotate camera upward (pitch)\n");
// 		scene->camera.direction = rotate_pitch(scene->camera.direction, scale);
// 	}
// 	else if (keycode == XK_l)
// 	{
// 		printf("L pressed: Rotate camera downward (pitch)\n");
// 		scene->camera.direction = rotate_pitch(scene->camera.direction, -scale);
// 	}
// 	else if (keycode == XK_z)
// 	{
// 	printf("Z pressed: Zoom in (decrease FOV)\n");
// 	if (scene->camera.fov > 10)
// 		scene->camera.fov -= 5;
// 	else
// 		printf("Minimum FOV reached\n");
// 	}
// 	else if (keycode == XK_x)
// 	{
// 		printf("X pressed: Zoom out (increase FOV)\n");
// 		if (scene->camera.fov < 130)
// 			scene->camera.fov += 5;
// 		else
// 			printf("Maximum FOV reached\n");
// 	}
// 	if ((keycode == XK_E || keycode == XK_e)
// 		&& r->mode != RENDER_REALTIME)
// 	{
// 		r->mode = RENDER_REALTIME;
// 		r->has_drawn_realtime = false;
// 		printf("Switched to REALTIME mode (edit)\n");
// 	}
// 	else if ((keycode == XK_R || keycode == XK_r)
// 		&& r->mode != RENDER_HIGH_QUALITY)
// 	{
// 		r->mode = RENDER_HIGH_QUALITY;
// 		r->render_done = false;
// 		printf("Switched to RENDER mode\n");
// 	}
// 	return (0);
// }
