#include "rt.h"

// PROVISOIRE
int	key_hook(int keycode, void *param)
{
	t_renderer	*r;

	r = g_renderer(NULL);
	(void)param;

	if (r->is_rendering)
		return (0);
	if (keycode == XK_Escape)
	{
		printf("Exiting...\n");
		//some free() function 
		exit(0);
	}
	if ((keycode == XK_E || keycode == XK_e)
		&& r->mode != RENDER_REALTIME)
	{
		r->mode = RENDER_REALTIME;
		printf("Switched to REALTIME mode (edit)\n");
	}
	else if ((keycode == XK_R || keycode == XK_r)
		&& r->mode != RENDER_HIGH_QUALITY)
	{
		r->mode = RENDER_HIGH_QUALITY;
		r->render_done = false;
		printf("Switched to RENDER mode\n");
	}
	return (0);
}
