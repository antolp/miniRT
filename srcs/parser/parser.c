/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:10:46 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/28 02:40:55 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//	***Sometime, Someday***
// For a fully autonomous dispatcher struct assigner
// Meant to replace the set_builder.c utilities
// At its core, a static array of stored "proceedures"
//  specified as an array of enums for a specified OBJ_TYPE
// Can also use a nested array / struct to store a flag
// for further complex behavior such as for an optional property
// !!!The following static array is completly disfunctional
// !!! and is there solely for illustration purposes
// static int	prop_format[][10][2] = {
// 	// [OBJ_CAMERA] = {{PROP_SIZE, true}, {PROP_COLOUR, true}},
// 	[OBJ_PLANE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_COLOUR, true}},
// 	[OBJ_SPHERE] = {{PROP_POSITION, true}, {PROP_SIZE, true}, {PROP_COLOUR, true}},
// 	[OBJ_CYLINDER] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_SIZE, true}, {PROP_SIZE, true}, {PROP_COLOUR, true}},
// 	[OBJ_CONE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_SIZE, true}, {PROP_COLOUR, true}},
// 	[OBJ_TRIANGLE] = {{PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_COLOUR, true}}
// };

t_parser	*parser_data(void)
{
	static t_parser	s = (t_parser){0};

	return (&s);
}

void	put_err(char *msg)
{
	ft_dprintf(STDERR_FILENO, msg);
	ft_putchar_fd('\n', STDERR_FILENO);
	clear_minirt();
	exit(EXIT_FAILURE);
}

//Check a given property against a range of values
//Cleaner print for future float / double handling in printf:
//ft_dprintf(STDERR_FILENO, "Expecting value for type %u
//between [%f] and [%f] for value %f\n",
//range.type, range.min, range.max, val);
void	check_range(double val, t_property_rules range)
{
	if (val < range.min || val > range.max)
	{
		ft_dprintf(STDERR_FILENO, "Out of range value.. Expected [min], [max] pairs:\n");
		ft_dprintf(STDERR_FILENO, " [POSITION] — -FLT_MAX, FLT_MAX\n [DIRECTION] — -1, 1\n");
		ft_dprintf(STDERR_FILENO, " [COLOUR] — 0, 255\n [RATIO] — 0.0, 1.0\n");
		ft_dprintf(STDERR_FILENO, " [ANGLE] — 0, 180\n [SIZE] — 0, FLT_MAX\n [PATH] — MIN: 4");
		put_err("");
	}
}

//Returns a *rule corresponding to a specific scene object
//Checks prop for a known object ENUM, 
//if undefined OBJ, checks the char* against litteral ids
t_asset_format	*get_asset_rules(char *type, t_object_type enm_type)
{
	size_t					i;
	static t_asset_format	assets[] = {
	[OBJ_AMBIANT_LIGHT] = {"A", 0, 0, 1, &build_ambiant_light, 0},
	[OBJ_CAMERA] = {"C", 0, 1, 1, &build_camera, 0},
	[OBJ_LIGHT] = {"L", 0, 0, SIZE_MAX, &build_light, 0},
	[OBJ_BACKGROUND] = {"BG", 0, 0, 1, &build_background, 0},
	[OBJ_SKYBOX] = {"SB", 0, 0, 1, &build_skybox, 0},
	[OBJ_SPHERE] = {"sp", 0, 0, SIZE_MAX, &build_sphere, 0},
	[OBJ_PLANE] = {"pl", 0, 0, SIZE_MAX, &build_plane, 0},
	[OBJ_CYLINDER] = {"cy", 0, 0, SIZE_MAX, &build_cylinder, 0},
	[OBJ_CONE] = {"co", 0, 0, SIZE_MAX, &build_cone, 0},
	[OBJ_TRIANGLE] = {"tr", 0, 0, SIZE_MAX, &build_triangle, 0}};

	if (enm_type != OBJ_UNKNOWN && enm_type < sizeof(assets) / sizeof(*assets))
		return (&assets[enm_type]);
	i = 1;
	while (i < sizeof(assets) / sizeof(*assets))
	{
		if (ft_strcmp(type, assets[i].type) == 0)
			return (assets[i].type_enum = i, &assets[i]);
		i++;
	}
	return (put_err("Unidentified asset type"), NULL);
}

void	parse_object(char **line)
{
	t_object		*object;
	t_asset_format	*rules;

	rules = get_asset_rules(*line++, 0);
	if (rules->type_enum <= OBJ_TRIANGLE)
	{
		object = malloc(sizeof(t_object));
		if (!object)
			put_err("Parser : Failled to malloc t_object");
		*object = (t_object){0};
		object->type = rules->type_enum;
		rules->shape_builder(object, line);
	}
	else
		rules->shape_builder(g_scene(NULL), line);
	rules->quantity++;
	return ;
}

void	parse_rtconfig(char *path) 
{
	int			fd;
	t_parser	*parser;

	parser = parser_data();
	if (ft_strlen(path) < 3 || !ft_strrnstr(path, ".rt", 3))
		put_err("Invalid file name");
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (put_err("Failled to open path"));
	while ((parser->line = get_next_line(fd)))
	{
		if (ft_strchr("#\n", *parser->line)) {
			free(parser->line); 
			continue ;
		}
		parser->word_arr = ft_split(parser->line, ' ');
		if (!parser->word_arr)
			put_err("Parsing : failled to malloc parser->word_arr");
		parse_object(parser->word_arr);
		ft_free_dynarr(parser->word_arr);
		free(parser->line);
		parser->word_arr = NULL;
	}
	if (get_asset_rules(NULL, OBJ_CAMERA)->quantity != 1)
		put_err("Invalid Camera count: expecting 1");
}