/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 09:34:39 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/26 01:59:24 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	build_ambiant_light(t_scene *set, char **line)
{
	//max 1
	printf("Scene color values %d, %d, %d\n With ptrs %p, %p, %p\n",
		 set->ambient_color.r, set->ambient_color.g, set->ambient_color.b,
		 &set->ambient_color.r, &set->ambient_color.g, &set->ambient_color.b);
	set_property(PROP_RATIO, &set->ambient_ratio, *line++);
	set_property(PROP_COLOUR, &set->ambient_color, *line++);
	//ratio
	//rgb
}

void	build_camera(t_scene *set, char **line)
{
	//max 1
	set_property(PROP_POSITION, &set->camera.position, *line++);
	set_property(PROP_DIRECTION, &set->camera.direction, *line++);
	set_property(PROP_ANGLE, &set->camera.fov, *line++);
	
	// set->camera.fov = 60;
	// (*line)++;
	//!!HANDLE OPTIONAL PROP !?!
	
	
	//exactly 1
	//position
	//direction
	//optional: angle
}

void	build_light(t_scene *set, char **line)
{
	t_light	*light;

	light = malloc(sizeof(t_light));
	if (!light)
		put_err("build_light: malloc error");
	ft_lstadd_back(&set->lights, add_content(light));
	*light = (t_light){0};
	set_property(PROP_POSITION, &light->position, *line++);
	set_property(PROP_RATIO, &light->intensity, *line++);
	set_property(PROP_COLOUR, &light->color, *line++);
	//as much as desired
	//position
	//ratio
	//rgb
}

void	build_background(t_scene *set, char **line)
{
	set_property(PROP_COLOUR, &set->background_color, *line++);
	//one or none
	//rgb
}

void	build_skybox(t_scene *set, char **line)
{
	set_property(PROP_PATH, &set->skybox, *line++);
	//one or none
	//path
}