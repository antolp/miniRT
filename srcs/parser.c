/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:10:46 by epinaud           #+#    #+#             */
/*   Updated: 2025/10/14 23:55:07 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	put_err(char *msg)
{
	ft_dprintf(STDERR_FILENO, msg);
	ft_putchar_fd('\n', STDERR_FILENO);
	exit(EXIT_FAILURE);
}

//
bool	parse_vec3(char *val, t_vec3 *vec)
{
	float	num;
	size_t	ret_atof;

	//parse x
	num = 0;
	ret_atof = ft_atof(val, &num);
	printf("Sum is now %f with len of %ld\n", num, ret_atof);
	if (!ret_atof || val[ret_atof] != ',')
		put_err("[vec3] Invalid data : Unexpected value or Missing coordinate\n");
	val += ret_atof + 1;
	vec->x = num;
	num = 0;
	//parse y
	ret_atof = ft_atof(val, &num);
	if (!ret_atof || val[ret_atof] != ',')
		put_err("[vec3] Invalid data : Unexpected value or Missing coordinate\n");
	val += ret_atof + 1;
	vec->y = num;
	num = 0;
	//parse z
	ret_atof = ft_atof(val, &num);
	if (!ret_atof)
		put_err("[vec3] Invalid data : Unexpected value or Missing coordinate\n");
	vec->z = num;
	return (1);
}

//Proposed structure : Each object has a builder function, each function knows which property is to be tested, each property has its checker / builder

//Charged to assert the integrity of given property value
bool	check_type(size_t prop, char **value) {
	float	sum;
	static t_property_rules	props[] = {
		[PROP_POSITION] = {PROP_POSITION, FLT_MIN, FLT_MAX},
		[PROP_DIRECTION] = {PROP_DIRECTION, -1, 1},
		[PROP_COLOUR] = {PROP_COLOUR, 0, 255},
		[PROP_BRIGHTNESS] = {PROP_BRIGHTNESS, 0.0, 1.0},
		[PROP_FOV] = {PROP_FOV, 0, 180},
		[PROP_DIMENSION] = {PROP_DIMENSION, FLT_MIN, FLT_MAX},
		// {PROP_DIRECTION, FLT_MIN, FLT_MAX},
		[PROP_PATH] = {PROP_PATH, FLT_MIN, FLT_MAX}
	};
	// static size_t	chkRange[] = {PROP_POSITION, PROP_DIRECTION, PROP_COLOUR,
	// 	PROP_BRIGHTNESS, PROP_FOV, PROP_DIMENSION };

	sum = 0;
	// ft_atof("   2546.5874lll", &sum);
	// printf("Atof result : %f\n", sum);
	if (prop == PROP_POSITION || prop == PROP_DIRECTION)
	{
		t_vec3	vec = {0};

		parse_vec3(*value, &vec);
		printf("String after vec3 parsing: %s\n", *value);
		if (PROP_DIRECTION && (!vec.x && !vec.y && !vec.z))
			put_err("Invalid vector dimensions : one axis should not be 0");
	}
	else if (prop == PROP_PATH)
	{
		if (ft_strlen(*value) < 4
		|| !ft_strnstr(*value + ft_strlen(*value) - 4, ".xpm", 4))
			put_err("Invalid file name: expecting *.xpm");
	}
	// if (in_array(prop, chkRange, nb_elems(chkRange, sizeof(chkRange)))) {
	// 	if ( sum < props[prop].min || sum > props[prop].max )
	// 		put_err("Parameter has out of range value");
	// }
	//Count and check object quantity
	return (1);
}

// void	check_params(t_asset *object, char **line) {
// 	size_t	i = 0;

// static t_property_rules	prop_format[][2] = {
// 	// [OBJ_CAMERA] = {{PROP_RATIO, true}, {PROP_COLOUR, true}},7
// 	[OBJ_PLANE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_COLOUR, true}},
// 	[OBJ_SPHERE] = {{PROP_POSITION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
// 	[OBJ_CYLINDER] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_DIMENSION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
// 	[OBJ_CONE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
// 	[OBJ_TRIANGLE] = {{PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_COLOUR, true}}
// };
// 	// static t_asset asset[] = {
// 	// 	{.type_id = "Test", .params = {"param1", "param2", 0}}
// 	// };
// 	// asset->type_id = "Changed litteral";
// 	// printf("Asset 0 name : %s, Param 1 : %s Param 2 %s \n", asset->type_id, asset->params[0], asset->params[1]);
// 	while (**line != ' ')
// 	{
	
// 	}
	
// }

void	parse_object(char *line, size_t	*idx)
{
	//Get glob object t_scene
	size_t	i;
	static t_asset_format	assets[] = {
		// [OBJ_UNKNOWN] = {NULL},
		[OBJ_AMBIANT_LIGHT] = {"A", 0, 0, 1},
		[OBJ_CAMERA] = {"C", 0, 1, 1},
		[OBJ_LIGHT] = {"L", 0, 0, SIZE_MAX},
		[OBJ_BACKGROUND] = {"BG", 0, 0, 1},
		[OBJ_SKYBOX] = {"SB", 0, 0, 1},
		[OBJ_SPHERE] = {"sp", 0, 0, SIZE_MAX},
		[OBJ_PLANE] = {"pl", 0, 0, SIZE_MAX},
		[OBJ_CYLINDER] = {"cy", 0, 0, SIZE_MAX},
		[OBJ_CONE] = {"co", 0, 0, SIZE_MAX},
		[OBJ_TRIANGLE] = {"tr", 0, 0, SIZE_MAX, &build_triangle}};

	printf("Entering object parsing with line : %s\n", line[*idx]);
	while (line[*idx] == ' ')
	{
		// printf("Skipping [%c] char Remaining line %s\n", **line, *line);
		// (line)++;
		*idx++;		
	}
	i = 1;
	while (i < sizeof(assets) / sizeof(*assets))
	{
		printf("asset type is %s\n", assets[i].type);
		if (ft_strnstr(&line[*idx], assets[i].type, ft_strlen(assets[i].type)))
		{
			//Call asset parser | Asset is => assets[i]
			//Add back scene objects 
			assets[i].quantity++;
			*idx += ft_strlen(assets[i].type);
			printf("Asset %s found. Remaining line : %s\n", assets[i].type ,&line[*idx]);
			return ;
		}
		i++;
	}
	put_err("Unidentified asset type");
}

void	parse_rtconfig(char *path) 
{
	// size_t	i;
	int		fd;
	// char	*line = "0.1,0.4,6.78";
	char	*line = NULL;
	size_t	index;
	
	// printf("Passed chain : %s\n", path);
	// check_type(PROP_POSITION, &path);
	
	// check_type(PROP_POSITION, &somecharptr);
	// t_vec3	vec = {0};
	// parse_vec3(line, &vec);
	// printf("Vec x %f, y %f , z %f\n", vec.x, vec.y, vec.z);
	// check_type(PROP_AXIS, &line);
	// return ;
	printf("Path %s\n", path);
	if (ft_strlen(path) < 3
		|| !ft_strnstr(&path[ft_strlen(path) - 3], ".rt", 3))
			put_err("Invalid file name");
	// check path integrity
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (put_err("Failled to open path"));
	while (line = get_next_line(fd))
	{
		index = 0;
		printf("_>> %s\n", line);
		while (*line)
			parse_object(line, &index);
		free(line);
	}
	printf("Ceased to read file\n");
	//!!! CHECK THAT ALL MANDATORY ASSETS ARE SET IN THE SCENE
}