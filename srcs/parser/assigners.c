/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   assigners.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 22:57:46 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/28 17:15:26 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_property_rules	get_prop_rules(size_t type)
{
	static t_property_rules	props[] = {
	[PROP_POSITION] = {PROP_POSITION, -FLT_MAX, FLT_MAX},
	[PROP_DIRECTION] = {PROP_DIRECTION, -1, 1},
	[PROP_COLOUR] = {PROP_COLOUR, 0, 255},
	[PROP_RATIO] = {PROP_RATIO, 0.0, 1.0},
	[PROP_ANGLE] = {PROP_ANGLE, 0, 180},
	[PROP_SIZE] = {PROP_SIZE, 0, FLT_MAX},
	[PROP_PATH] = {PROP_PATH, 4, FLT_MAX},
	[PROP_UV] = {PROP_UV, 0, 2}};

	return (props[type]);
}

//Generic subparser mod that handles
// both float and int assignations into a t_vec3 struct
static void	parse_valset(void *valset[], t_property_type type, char *line)
{
	int	ret_atof;

	ret_atof = 0;
	while (*valset)
	{
		if (type == PROP_COLOUR)
			ret_atof = ft_atoi2(line, *valset);
		else if (type == PROP_POSITION || type == PROP_DIRECTION)
			ret_atof = ft_atof(line, *valset);
		if (ret_atof < 1 || !ft_strchr(";,\n\0 ", line[ret_atof]))
			put_err("Invalid data : Unexpected or Missing value\n");
		if (type == PROP_COLOUR)
			check_range(**(int **)valset, get_prop_rules(type));
		else if (type == PROP_POSITION || type == PROP_DIRECTION)
			check_range(**(double **)valset, get_prop_rules(type));
		if (line[ret_atof] == ',')
			ret_atof += 1;
		line += ret_atof;
		valset++;
	}
}

static void	checker_assigner(t_texture *dst, char *line)
{
	t_checkerboard	*cb;

	cb = malloc(sizeof(t_checkerboard));
	if (!cb)
		put_err("Malloc faillure: t_checkerboard");
	*cb = (t_checkerboard){0};
	*dst = (t_texture){.type = TEXTURE_CHECKER, .data = cb};
	parse_valset((void *[]){&cb->color1.r, &cb->color1.g, &cb->color1.b, 0},
		PROP_COLOUR, line);
	line = ft_strchr(line, ';');
	if (!line || *line != ';')
		put_err(ERR_CHECKER_FORMAT);
	parse_valset((void *[]){&cb->color2.r, &cb->color2.g, &cb->color2.b, 0},
		PROP_COLOUR, ++line);
	line = ft_strchr(line, ';');
	if (!line || *line != ';')
		put_err(ERR_CHECKER_FORMAT);
	set_property(PROP_SIZE, &cb->scale_u, ++line);
	line = ft_strchr(line, ';');
	if (!line || *line != ';')
		put_err(ERR_CHECKER_FORMAT);
	set_property(PROP_SIZE, &cb->scale_v, ++line);
}

static void	assign_path(t_texture *img, char *line)
{
	strstripchr(line, "\n", ft_strlen(line));
	if (ft_strlen(line) < 4
		|| !ft_strnstr(line + ft_strlen(line) - 4, ".xpm", 4))
		put_err("Invalid file name: expecting *.xpm");
	img->type = TEXTURE_IMAGE;
	img->data = load_xpm_image(g_renderer(NULL)->mlx, line);
	if (!img->data)
		put_err("XPM image loading: faillure");
}

//Rooter charged to determine the relevant assigner 
//and assert the integrity of and for a given property value
bool	set_property(size_t type, void *dst, char *line)
{
	if (!line)
		put_err("Invalid data : missing parameter");
	if (type == PROP_POSITION || type == PROP_DIRECTION)
	{
		parse_valset((void *[]){&((t_vec3 *)dst)->x, &((t_vec3 *)dst)->y,
			&((t_vec3 *)dst)->z, 0}, type, line);
		if (type == PROP_DIRECTION && (!((t_vec3 *)dst)->x
				&& !((t_vec3 *)dst)->y && !((t_vec3 *)dst)->z))
			put_err("Invalid vector dimensions : one axis should not be 0");
	}
	else if (type == PROP_COLOUR)
		parse_valset((void *[]){&((t_color *)dst)->r, &((t_color *)dst)->g,
			&((t_color *)dst)->b, 0}, type, line);
	else if (type == PROP_PATH)
		assign_path(dst, line);
	else if (type == PROP_CHECKER)
		checker_assigner(dst, line);
	else
	{
		if (ft_atof(line, (double *)dst) < 1)
			put_err("[Atof-Atoi2] : Overflow or parsing error");
		check_range(*(double *)dst, get_prop_rules(type));
	}
	return (EXIT_SUCCESS);
}
