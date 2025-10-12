/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:10:46 by epinaud           #+#    #+#             */
/*   Updated: 2025/10/12 18:40:29 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
typedef struct s_asset {
	char			*type_id;
	t_object_type	params[5];
	size_t			count;
}	t_asset;

void	put_err(char *msg)
{
	ft_dprintf(STDERR_FILENO, msg);
	ft_putchar_fd(STDERR_FILENO, '\n');
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

typedef struct s_property_rules {
	t_object_type	type_id;
	double			val_min;
	double			val_max;
}	t_property_rules;


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
		// {PROP_DIRECTION, FLT_MIN, FLT_MAX},
		[PROP_PATH] = {PROP_PATH, FLT_MIN, FLT_MAX}
	};
	// static size_t	chkRange[] = {PROP_POSITION, PROP_DIRECTION, PROP_COLOUR,
	// 	PROP_BRIGHTNESS, PROP_FOV, PROP_DIMENSION };

	sum = 0;
	ft_atof("   2546.5874lll", &sum);
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
		|| !ft_strnstr(ft_strlen(*value) - 4, ".xpm", 4))
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
// 	// static t_asset asset[] = {
// 	// 	{.type_id = "Test", .params = {"param1", "param2", 0}}
// 	// };
// 	// asset->type_id = "Changed litteral";
// 	// printf("Asset 0 name : %s, Param 1 : %s Param 2 %s \n", asset->type_id, asset->params[0], asset->params[1]);
// 	while (**line != ' ')
// 	{
	
// 	}
	
// }

void	parse_object(char **line)
{
	static const size_t	asset_count = 10;
	size_t	i;
	static char	*assets[] = {
		[0] = "A", [1] = "C", [2] = "L", [3] = "BG", 
		[4] = "SB",	[5] = "sp", [6] = "pl",
		[7] = "cy", [8] = "co", [9] = "tr"
	};
	/* 	size_t			quantity;
	size_t			qtmin;
	size_t			qtmax;
	 */
	static t_object_props	prop_format[][2] = {
		// [OBJ_CAMERA] = {{PROP_RATIO, true}, {PROP_COLOUR, true}},7
		[OBJ_PLANE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_COLOUR, true}},
		[OBJ_SPHERE] = {{PROP_POSITION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
		[OBJ_CYLINDER] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_DIMENSION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
		[OBJ_CONE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
		[OBJ_TRIANGLE] = {{PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_COLOUR, true}}
	};
	//Element
	//Priority
	//Unicity

	//Get glob object t_scene

	while (**line == ' ')
		*line++;
	i = 0;
	
	while (assets[i])
	{
		if (i >= asset_count)
			put_err("Unidentified asset type");
		if (ft_strnstr(*line, assets[i], ft_strlen(assets[i])))
			//Call asset parser
			//Add back scene objects 
			break ;
		i++;
	}
	*line += ft_strlen(assets[i]);
	printf("Remaining line : %s\n", *line);
}

void	parse_rtconfig(char *path) 
{
	// size_t	i;
	int		fd;
	// char	*line = "0.1,0.4,6.78";
	char	*line = NULL;
	
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
		printf("_%s\n");
		while (*line)
			parse_object(&line);
	}

	//!!! CHECK THAT ALL MANDATORY ASSETS ARE SET IN THE SCENE
}