/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/28 22:16:44 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//getset immutable alternative
//checking set makes the function rebindable if not called with NULL
//checking !r makes the function return r everytime after first passed set ptr
//(theorically safer)
//also changed init param from &(t_var){} to &var with var living on main stack
t_renderer	*g_renderer(t_renderer *set)
{
	static t_renderer	*r = NULL;

	if (!r)
		r = set;
	return (r);
}

t_scene	*g_scene(t_scene *set)
{
	static t_scene	*s = NULL;

	if (!s)
		s = set;
	return (s);
}

//here, add some free() mechanism
int	handle_close(void *param)
{
	(void)param;
	printf("Exiting...\n");
	clear_minirt();
	exit(0);
	return (0);
}

// int	main(int argc, char	*argv[])
// {
// 	t_scene		scene_g;
// 	t_renderer	renderer_g;

// 	if (argc != 2)
// 	{
// 		ft_dprintf(2, "Insufficient parameter count :");
// 		ft_dprintf(2, " .rt config file path might be missing\n");
// 		exit(1);
// 	}
// 	g_scene(&scene_g);
// 	scene_g = (t_scene){0};
// 	g_renderer(&renderer_g);
// 	if (!init_renderer(&renderer_g, WIDTH, HEIGHT, "rt"))
// 		put_err("Renderer init faillure");
// 	// parse_rtconfig(argv[1]);
// 	init_scene(&scene_g);
// 	// print_scene();
// 	mlx_hook(renderer_g.win, KeyPress, KeyPressMask, key_hook, NULL);
// 	mlx_loop_hook(renderer_g.mlx, render_loop, NULL);
// 	mlx_hook(renderer_g.win, DestroyNotify, 0, handle_close, NULL);
// 	mlx_loop(renderer_g.mlx);
// 	(void)(argv);
// 	return (0);
// }
int	main(int argc, char	*argv[])
{
	t_scene		scene_g;
	t_renderer	renderer_g;

	if (argc != 2)
	{
		ft_dprintf(2, "Insufficient parameter count :");
		ft_dprintf(2, " .rt config file path might be missing\n");
		exit(1);
	}
	g_scene(&scene_g);
	scene_g = (t_scene){0};
	g_renderer(&renderer_g);
	if (!init_renderer(&renderer_g, WIDTH, HEIGHT, "rt"))
		put_err("Renderer init faillure");
	parse_rtconfig(argv[1]);
	mlx_hook(renderer_g.win, KeyPress, KeyPressMask, key_hook, NULL);
	mlx_loop_hook(renderer_g.mlx, render_loop, NULL);
	mlx_hook(renderer_g.win, DestroyNotify, 0, handle_close, NULL);
	mlx_loop(renderer_g.mlx);
	(void)(argv);
	return (0);
}
