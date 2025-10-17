/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:10:46 by epinaud           #+#    #+#             */
/*   Updated: 2025/10/19 19:29:41 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	clear_content(t_list *elm)
{
	if (((t_object *)elm->content)->shape)
		free(((t_object *)elm->content)->shape);
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
	exit(EXIT_FAILURE);
}

//>> Warning for char ending atof call ?
bool	parse_vec3(char *val, t_vec3 *vec)
{
	float	num;
	size_t	ret_atof;

	//parse x
	num = 0;
	ret_atof = ft_atof(val, &num);
	// printf("Sum is now %f with len of %ld\n", num, ret_atof);
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

//!!!!> Proposed structure : Each object has a builder function, each function knows which property is to be tested, each property has its checker / builder

//Charged to assert the integrity of given property value
bool	set_property(size_t type, void *dst, char **line)
{
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
	if (type == PROP_POSITION || type == PROP_DIRECTION)
	{
		t_vec3	*vec = dst;

		parse_vec3(*line, vec);
		printf("String after vec3 parsing: %s\n", *line);
		if (PROP_DIRECTION && (!vec->x && !vec->y && !vec->z))
			put_err("Invalid vector dimensions : one axis should not be 0");
	}
	else if (type == PROP_PATH)
	{
		if (ft_strlen(*line) < 4
		|| !ft_strnstr(*line + ft_strlen(*line) - 4, ".xpm", 4))
			put_err("Invalid file name: expecting *.xpm");
	}
	// if (in_array(prop, chkRange, nb_elems(chkRange, sizeof(chkRange)))) {
	// 	if ( sum < props[prop].min || sum > props[prop].max )
	// 		put_err("Parameter has out of range value");
	// }
	//Count and check object quantity
	return (1);
}


// void	init_shape(t_object_type type, t_object	*obj)
// {
// 	switch (type)
// 	{
// 		case OBJ_PLANE:
// 			obj->shape = malloc(sizeof(t_plane));
// 		break;
// 		case OBJ_SPHERE:
// 			obj->shape = malloc(sizeof(t_sphere));
// 		break;
// 		case OBJ_CYLINDER:
// 			obj->shape = malloc(sizeof(t_cylinder));
// 		break;
// 		case OBJ_CONE:
// 			obj->shape = malloc(sizeof(t_cone));
// 		break;
// 		case OBJ_TRIANGLE:
// 			obj->shape = malloc(sizeof(t_triangle));
// 		break;
// 		}
		
// 	if (!obj->shape)
// 		put_err("build_triangle : Failled to malloc shape");
// }

//Will validate the integrity of user provided parameters for the specified shape
//-->>Will be called from shape builders
void	check_params(t_object_type type, char **line) {
	size_t	i = 0;

	static int	prop_format[][10][2] = {
		// [OBJ_CAMERA] = {{PROP_RATIO, true}, {PROP_COLOUR, true}},7
		[OBJ_PLANE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_COLOUR, true}},
		[OBJ_SPHERE] = {{PROP_POSITION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
		[OBJ_CYLINDER] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_DIMENSION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
		[OBJ_CONE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
		[OBJ_TRIANGLE] = {{PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_COLOUR, true}}
	};

	while (1)
	{
		//>>> Call the (proper?) param handler
		// prop_format[type][0];
		// idx += ft_strchr(line + *idx, ' ') - &line[*idx];
		break ;
	}
	//Should perhaps return the material object filled with its proper values
}

void	parse_object(char **line)
{
	size_t					i;
	t_object				*object;
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

	i = 1;
	while (i < sizeof(assets) / sizeof(*assets))
	{
		printf("asset type is %s\n", assets[i].type);
		if (ft_strcmp(*line, assets[i].type) == 0)
		{
			printf("Asset %s found. Remaining line :\n", assets[i].type);
			line++;
			put_recurse_dynarr(line);
			object = malloc(sizeof(t_object));
			if (!object)
				put_err("Parser : Failled to malloc t_object");
			object->type = i;
			assets[i].shape_builder(object, line);
			assets[i].quantity++;
			return ;
		}
		i++;
	}
	put_err("Unidentified asset type");
}

void	parse_rtconfig(char *path) 
{
	int		fd;
	char	*line = NULL;
	char	**word_list = NULL;
	size_t	index;
	
	// printf("Passed chain : %s\n", path);
	// check_type(PROP_POSITION, &path);
	// check_type(PROP_POSITION, &somecharptr);
	// check_type(PROP_AXIS, &line);
	printf("Path %s of size %zu\n", path, ft_strlen(path));
	if (ft_strlen(path) < 3
		|| !ft_strnstr(&path[ft_strlen(path) - 3], ".rt", 3))
			put_err("Invalid file name");
	// check path integrity
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (put_err("Failled to open path"));
	while ((line = get_next_line(fd)))
	{
		if (!line)
			break ;
		index = 0;
		printf("_>> %s\n", line);
		word_list = ft_split(line, ' ');
		free(line);
		if (!word_list)
			put_err("Parsing : failled to malloc word_list");
		parse_object(word_list);
		ft_free_dynarr(word_list);
		word_list = NULL;
	}
	printf("Ceased to read file\n");
	//!!! CHECK THAT ALL MANDATORY ASSETS ARE SET IN THE SCENE
	ft_lstclear(&g_scene(0)->objects, clear_content);
}