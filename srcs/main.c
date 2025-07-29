/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

#define WIDTH 800
#define HEIGHT 600

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
	static t_scene *s = NULL;

	if (!s)
		s = set;
	return (s);
}


char	*test()
{
	return("test123");
}

void	g_test()
{
	printf("\ncoucou : %s\n", (char *)g_renderer(NULL)->mlx);
}







int	main(void)
{
	t_scene		scene_g;
	t_renderer	renderer_g;

	if (!init_renderer(&renderer_g, WIDTH, HEIGHT, "rt"))
		return (1); // need to aadd a free() mechanism 
	g_renderer(&renderer_g);
	if (!init_scene(&scene_g))
		return (1); // again, need to aadd a free() mechanism
	g_scene(&scene_g);
	mlx_hook(renderer_g.win, KeyPress, KeyPressMask, key_hook, NULL);
	// mlx_key_hook(renderer_g.mlx, key_hook, NULL);
	mlx_loop_hook(renderer_g.mlx, render_loop, NULL);
	mlx_loop(renderer_g.mlx);
	return (0);
}