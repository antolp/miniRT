#include "rt.h"

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

int	handle_edit_menu_keys_wrapper(int keycode, t_renderer *r)
{
	if (r->mode == RENDER_EDIT_MENU)
	{
		handle_menu_keys(keycode, r);
		return (1);
	}
	return (0);
}

int	handle_open_flags_menu(int keycode, t_renderer *r)
{
	if (keycode == XK_m && r->mode == RENDER_EDIT)
	{
		enter_flags_menu(r);
		return (1);
	}
	return (0);
}
