#include "rt.h"

int	handle_mode_switch(int keycode, t_renderer *r)
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

int	handle_quit(int keycode)
{
	if (keycode == XK_Escape)
	{
		g_renderer(NULL)->should_quit = true;
		printf("Exiting...\n");
		exit(0);
	}
	return (0);
}

int	handle_supersample_toggle(int keycode, t_renderer *r)
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