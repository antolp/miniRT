/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug_objects.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 04:57:48 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 05:05:26 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	print_sphere(t_sphere *s)
{
	printf("sphere\n");
	if (!s)
	{
		printf("No sphere !\n");
		return ;
	}
	print_vec3("\tcenter", s->center);
	printf("\tradius : \t%f\n", s->radius);
}

void	print_plane(t_plane *p)
{
	printf("plane\n");
	if (!p)
	{
		printf("No plane !\n");
		return ;
	}
	print_vec3("\tpoint ", p->point);
	print_vec3("\tnormal", p->normal);
}

void	print_cylinder(t_cylinder *cyl)
{
	printf("cylinder\n");
	if (!cyl)
	{
		printf("No cylinder !\n");
		return ;
	}
	print_vec3("\tcenter", cyl->center);
	print_vec3("\taxis", cyl->axis);
	printf("\tradius :\t%f\n", cyl->radius);
	printf("\theight :\t%f\n", cyl->height);
}

void	print_cone(t_cone *co)
{
	printf("cone\n");
	if (!co)
	{
		printf("No cone !\n");
		return ;
	}
	print_vec3("\tapex", co->apex);
	print_vec3("\taxis", co->axis);
	printf("\tangle : \t%f\n", co->angle);
	printf("\theight :\t%f\n", co->height);
}

void	print_triangle(t_triangle *t)
{
	printf("triangle\n");
	if (!t)
	{
		printf("No triangle !\n");
		return ;
	}
	print_vec3("\tp0", t->p0);
	print_vec3("\tp1", t->p1);
	print_vec3("\tp2", t->p2);
	printf("\tuv mode : %i", t->uv_mode);
	printf("\n");
}
