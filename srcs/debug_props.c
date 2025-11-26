/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_props.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 04:57:51 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 05:09:33 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	print_vec3(char *s, t_vec3 v)
{
	if (ft_strlen(s) > 5)
		printf("%s :\t(%.2f, %.2f, %.2f)\n", s, v.x, v.y, v.z);
	else
		printf("%s :\t\t(%.2f, %.2f, %.2f)\n", s, v.x, v.y, v.z);
}

void	print_color(char *s, t_color c)
{
	if (ft_strlen(s) > 5)
		ft_printf("%s :\t(R: %d, G: %d, B: %d)\n", s, c.r, c.g, c.b);
	else
		ft_printf("%s :\t\t(R: %d, G: %d, B: %d)\n", s, c.r, c.g, c.b);
}

void	print_texture(char *s, t_texture tex)
{
	printf("%stype : %i\n", s, tex.type);
	printf("%stex pl addr : %p\n", s, tex.data);
}

void	print_camera(t_camera *c)
{
	if (!c)
		printf("No camera !\n");
	printf("--- CAMERA -------------------------\n");
	print_vec3("pos", c->position);
	print_vec3("dir", c->direction);
	printf("fov :\t\t%.2f deg\n", c->fov);
	printf("------------------------------------\n");
}
