/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/18 09:34:39 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/27 01:18:35 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	build_ambiant_light(t_scene *set, char **line)
{
	printf("Scene color values %d, %d, %d\n With ptrs %p, %p, %p\n",
		 set->ambient_color.r, set->ambient_color.g, set->ambient_color.b,
		 &set->ambient_color.r, &set->ambient_color.g, &set->ambient_color.b);
	if (get_asset_rules(NULL, OBJ_AMBIANT_LIGHT)->quantity > 0)
		put_err("Invalid Ambiant_Light count: one max");
	set_property(PROP_RATIO, &set->ambient_ratio, *line++);
	set_property(PROP_COLOUR, &set->ambient_color, *line++);
}

void	build_camera(t_scene *set, char **line)
{
	if (get_asset_rules(NULL, OBJ_CAMERA)->quantity != 0)
		put_err("Invalid Camera count: expecting 1");
	set_property(PROP_POSITION, &set->camera.position, *line++);
	set_property(PROP_DIRECTION, &set->camera.direction, *line++);
	set_property(PROP_ANGLE, &set->camera.fov, *line++);
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
}

void	build_background(t_scene *set, char **line)
{
	if (get_asset_rules(NULL, OBJ_BACKGROUND)->quantity > 0)
		put_err("Invalid Background count: one max");
	set_property(PROP_COLOUR, &set->background_color, *line++);
}

void	build_skybox(t_scene *set, char **line)
{
	if (get_asset_rules(NULL, OBJ_SKYBOX)->quantity > 0)
		put_err("Invalid Skybox count: one max");
	set_property(PROP_PATH, &set->skybox, *line++);
}