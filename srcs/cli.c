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

//bit table
//global, so not norm compliant but ill fix that
static const uint32_t	g_bits[FLAG_COUNT] = {
	FLAG_SHADOW, FLAG_TRANSPARENT_SHADOW, FLAG_DIFFUSE,
	FLAG_SPECULAR, FLAG_REFLECTION, FLAG_REFRACTION,
	FLAG_FRESNEL, FLAG_TEXTURE, FLAG_BUMP_MAP
};

//same
static const char		*g_names[FLAG_COUNT] = {
	"shadow", "transparent shadow", "diffuse",
	"specular", "reflection", "refraction",
	"fresnel", "texture", "bump map"
};

//these wont stay static
static void	print_flags_table(uint32_t mask)
{
	int		i;
	uint32_t	bit;

	printf("\n=== FLAGS MENU ===\n");
	printf("Press 1..9 to toggle. Press 'm' to close.\n\n");
	i = 0;
	while (i < FLAG_COUNT)
	{
		bit = g_bits[i];
		if ((mask & bit) != 0u)
			printf("[%d] %-20s : ON\n", i + 1, g_names[i]);
		else
			printf("[%d] %-20s : OFF\n", i + 1, g_names[i]);
		i++;
	}
	printf("\n");
	fflush(stdout);
}

static int	key_to_index(int keycode)
{
	if (keycode == XK_1)
		return (0);
	else if (keycode == XK_2)
		return (1);
	else if (keycode == XK_3)
		return (2);
	else if (keycode == XK_4)
		return (3);
	else if (keycode == XK_5)
		return (4);
	else if (keycode == XK_6)
		return (5);
	else if (keycode == XK_7)
		return (6);
	else if (keycode == XK_8)
		return (7);
	else if (keycode == XK_9)
		return (8);
	return (-1);
}

static void	toggle_index(uint32_t *mask, int index)
{
	uint32_t	bit;

	if (index < 0)
		return ;
	bit = g_bits[index];
	if ((*mask & bit) != 0u)
	{
		*mask &= ~bit;
		printf("%s off.\n", g_names[index]);
	}
	else
	{
		*mask |= bit;
		printf("%s on.\n", g_names[index]);
	}
	print_flags_table(*mask);
}

void	enter_flags_menu(t_renderer *r)
{
	r->mode = RENDER_EDIT_MENU;
	printf("Opened flags menu.\n");
	print_flags_table(*(uint32_t *)&r->shading_flag);
}

void	exit_flags_menu(t_renderer *r)
{
	r->mode = RENDER_EDIT;
	printf("Closed flags menu. Back to edit.\n");
}

//handles a key while in the flag menu
//could (should) be generalized into a main menu and sub menus
void	handle_menu_keys(int keycode, t_renderer *r)
{
	int			index;
	uint32_t	*mask;

	if (keycode == XK_m || keycode == XK_M)
	{
		exit_flags_menu(r);
		return ;
	}
	if (keycode == XK_r || keycode == XK_R)
	{
		printf("Close the menu before rendering.\n");
		return ;
	}
	index = key_to_index(keycode);
	mask = (uint32_t *)&r->shading_flag;
	if (index >= 0)
		toggle_index(mask, index);
}

//Will be rewritten to match the norm eventually
//maybe (surely) will be extended to live-edit the scene
int	key_hook(int keycode, void *param)
{
	t_renderer	*r;
	t_scene		*scene;

	(void)param;
	r = g_renderer(NULL);
	scene = g_scene(NULL);
	if (r->is_rendering)
		return (0);
	
	//var for yaw and reg "equator" rotations
	const t_vec3 world_up = {0, 1, 0};
	t_vec3 right = vec_normalize(vec_cross(scene->camera.direction, world_up));
	double scale = 3.0;

	//quit
	if (keycode == XK_Escape)
	{
		clear_minirt();
		printf("Exiting...\n");
		exit(0);
	}

	//menu takes over until it exits
	if (r->mode == RENDER_EDIT_MENU)
	{
		handle_menu_keys(keycode, r);
		return (0);
	}

	//mode switch, 	EDIT to HQ (now takes menu into account)
	//idk but i think this can be simplified
	if ((keycode == XK_E || keycode == XK_e) && r->mode != RENDER_EDIT)
	{
		r->mode = RENDER_EDIT;
		r->has_drawn_realtime = false;
		printf("Switched to REALTIME mode (edit)\n");
		return (0);
	}
	else if ((keycode == XK_R || keycode == XK_r) && r->mode != RENDER_HIGH_QUALITY)
	{
		if (r->mode != RENDER_EDIT_MENU)
		{
			r->mode = RENDER_HIGH_QUALITY;
			r->render_done = false;
			printf("Switched to RENDER mode\n");
		}
		return (0);
	}

	//opens menu if on edit mode (realtime non-shaded rendering)
	if (keycode == XK_m && r->mode == RENDER_EDIT)
	{
		enter_flags_menu(r);
		return (0);
	}

	//this doesnt seem to be useful, i'll let it here until the eventual rewrite
	// if (r->mode != RENDER_EDIT)
	// 	return (0);

	//supersampled is now 'u', and not 'm'
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
		return (0);
	}


	// === EDIT MODE CAMERA CONTROLS (and stuff)===
	if (r->mode != RENDER_EDIT)
		return (0);
	//MOVE camera
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
		scene->camera.position = vec_add(scene->camera.position, vec_mul(world_up, scale * 0.075));
		r->has_drawn_realtime = false;
	}
	else if (keycode == XK_g)
	{
		printf("G pressed: Move camera down\n");
		scene->camera.position = vec_sub(scene->camera.position, vec_mul(world_up, scale * 0.075));
		r->has_drawn_realtime = false;
	}
	//ROTATE (along equator)
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
	//YAW (rotate along hemisphere)
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
	//FOV control
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
	//SSAA supersampling
	//moved from very cool key 'm' to very inferior key 'u' (next to 's' un "super")
	else if (keycode == XK_u)
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
	// === END OF EDIT MODE CAMERA CONTROL (and stuff) ===
	
	//camera and supersampling


	else if (keycode == XK_b)
		printf("flag : %i\n", r->shading_flag);
	return (0);
}
