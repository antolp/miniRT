/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:10:46 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/22 22:57:43 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_parser	*parser_data(void)
{
	static t_parser s = (t_parser){0};

	return (&s);
}

void	clear_content(t_list *elm)
{
	t_object	*obj;

	if (!elm->content)
		return ;
	obj = elm->content;
	
	if (obj->shape)
		free(obj->shape);
	if (obj->material.texture.data)
		free(obj->material.texture.data);
	if (elm->content)
		free(elm->content);
}

void	clear_lights(t_list *elm)
{
	if (!elm->content)
		return ;
	if (elm->content)
		free(elm->content);
}

void	put_err(char *msg)
{
	// #include <errno.h>
	// perror("open"); printf("open failed: %s\n", strerror(errno));
	ft_dprintf(STDERR_FILENO, msg);
	ft_putchar_fd('\n', STDERR_FILENO);
	ft_lstclear(&g_scene(0)->objects, clear_content);
	ft_lstclear(&g_scene(0)->lights, clear_lights);
	if (parser_data()->line)
		free(parser_data()->line);
	if (parser_data()->word_arr)
		ft_free_dynarr(parser_data()->word_arr);
	exit(EXIT_FAILURE);
}

//Count and check object quantity
void	check_range(double val, t_property_rules range)
{
	// printf("Checking range for %f: [%f < > %f]\n", val, range.min, range.max);
	if (val < range.min || val > range.max)
	{
		ft_dprintf(STDERR_FILENO, "Expecting value for type %u between [%f] and [%f]: ", range.type, range.min, range.max);
		put_err("Parameter has out of range value");
	}
}

// Will validate the integrity of user provided parameters for the specified shape
// -->>Will be called from shape builders
// static int	prop_format[][10][2] = {
// 	// [OBJ_CAMERA] = {{PROP_SIZE, true}, {PROP_COLOUR, true}},
// 	[OBJ_PLANE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_COLOUR, true}},
// 	[OBJ_SPHERE] = {{PROP_POSITION, true}, {PROP_SIZE, true}, {PROP_COLOUR, true}},
// 	[OBJ_CYLINDER] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_SIZE, true}, {PROP_SIZE, true}, {PROP_COLOUR, true}},
// 	[OBJ_CONE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_SIZE, true}, {PROP_COLOUR, true}},
// 	[OBJ_TRIANGLE] = {{PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_COLOUR, true}}
// };
void	parse_object(char **line)
{
	size_t					i;
	t_object				*object;
	static t_asset_format	assets[] = {
	[OBJ_AMBIANT_LIGHT] = {"A", 0, 0, 1, &build_ambiant_light},
	[OBJ_CAMERA] = {"C", 0, 1, 1, &build_camera},
	[OBJ_LIGHT] = {"L", 0, 0, SIZE_MAX, &build_light},
	[OBJ_BACKGROUND] = {"BG", 0, 0, 1, &build_background},
	[OBJ_SKYBOX] = {"SB", 0, 0, 1, &build_skybox},
	[OBJ_SPHERE] = {"sp", 0, 0, SIZE_MAX, &build_sphere},
	[OBJ_PLANE] = {"pl", 0, 0, SIZE_MAX, &build_plane},
	[OBJ_CYLINDER] = {"cy", 0, 0, SIZE_MAX, &build_cylinder},
	[OBJ_CONE] = {"co", 0, 0, SIZE_MAX, &build_cone},
	[OBJ_TRIANGLE] = {"tr", 0, 0, SIZE_MAX, &build_triangle}};

	i = 1;
	while (i < sizeof(assets) / sizeof(*assets))
	{
		if (ft_strcmp(*line, assets[i].type) == 0)
		{
			printf("Asset %s found as index[%ld]. Remaining line :\n", assets[i].type, i);
			line++;
			put_recurse_dynarr(line);
			if (i < OBJ_TRIANGLE)
			{
				object = malloc(sizeof(t_object));
				if (!object)
					put_err("Parser : Failled to malloc t_object");
				*object = (t_object){0};
				object->type = i;
				assets[i].shape_builder(object, line);
				assets[i].quantity++;
			}
			else
				assets[i].shape_builder(g_scene(NULL), line);
			return ;
		}
		i++;
	}
	put_err("Unidentified asset type");
}

void	parse_rtconfig(char *path) 
{
	int		fd;
	size_t	index;
	t_parser	*parser;

	parser = parser_data();
	printf("Path %s of size %zu\n", path, ft_strlen(path));
	if (ft_strlen(path) < 3 || !ft_strrnstr(path, ".rt", 3))
		// || !ft_strnstr(&path[ft_strlen(path) - 3], ".rt", 3))
		put_err("Invalid file name");
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (put_err("Failled to open path"));
	while ((parser->line = get_next_line(fd)))
	{
		if (ft_strchr("#\n", *parser->line)) {
			free(parser->line); 
			continue ; }
		index = 0;
		printf("_>> %s\n", parser->line);
		parser->word_arr = ft_split(parser->line, ' ');
		if (!parser->word_arr)
			put_err("Parsing : failled to malloc parser->word_arr");
		parse_object(parser->word_arr);
		ft_free_dynarr(parser->word_arr);
		free(parser->line);
		parser->word_arr = NULL;
	}
	printf("Ceased to read file\n");
	//!!! CHECK THAT ALL MANDATORY ASSETS ARE SET IN THE SCENE
}