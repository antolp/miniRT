/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:10:46 by epinaud           #+#    #+#             */
/*   Updated: 2025/10/27 10:12:01 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

typedef struct s_parser
{
	char	*line;
	char	**word_arr;
}	t_parser;


t_parser	*parser_data(void)
{
	static t_parser s = (t_parser){0};

	return (&s);
}

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
	if (parser_data()->line)
		free(parser_data()->line);
	if(parser_data()->word_arr)
		ft_free_dynarr(parser_data()->word_arr);
	exit(EXIT_FAILURE);
}

//>> Warning for char ending atof call ?
void	parse_valset(char *line, void *valset[], t_property_type type)
{
	size_t	ret_atof;
	
	ret_atof = 0;
	while (*valset) 
	{
		if (type == PROP_COLOUR)
			ret_atof = ft_atoi2(line, *valset);
		else if (type == PROP_POSITION || type == PROP_DIRECTION)
			ret_atof = ft_atof(line, *valset);

		if (type == PROP_COLOUR)
			printf("Sum is now %d with len of %ld\n", **(int **)valset, ret_atof);
		else if (type == PROP_POSITION || type == PROP_DIRECTION)
			printf("Sum is now %f with len of %ld\n", **(double **)valset, ret_atof);

		if (!ret_atof || !ft_strchr(",\n\0", line[ret_atof]))
			put_err("Invalid data : Unexpected or Missing value\n");
		line += ret_atof + 1;
		valset++;
	}
}

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
	
	printf(" >>> Calling setproperty with line %s\n", *line);
	if (!*line)
		put_err("Invalid data : missing parameter");
	sum = 0;
	if (type == PROP_POSITION || type == PROP_DIRECTION)
	{
		t_vec3	*vec = dst;

		// parse_vec3(*line, dst);
		parse_valset(*line, (void *[]){&vec->x, &vec->y, &vec->z, 0}, PROP_DIRECTION);
		// printf("String after vec3 parsing: %s\n", *line);
		if (PROP_DIRECTION && (!vec->x && !vec->y && !vec->z))
			put_err("Invalid vector dimensions : one axis should not be 0");
		printf("Valset coordinates : %f,%f,%f \n",
		vec->x, vec->y, vec->z);
	}
	else if (type == PROP_COLOUR)
		parse_valset(*line, (void *[]){&((t_color *)dst)->r, &((t_color *)dst)->g, &((t_color *)dst)->b, 0}, PROP_COLOUR);
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

//Will validate the integrity of user provided parameters for the specified shape
//-->>Will be called from shape builders
// void	check_params(t_object_type type, char **line) {
// 	size_t	i = 0;

// 	static int	prop_format[][10][2] = {
// 		// [OBJ_CAMERA] = {{PROP_RATIO, true}, {PROP_COLOUR, true}},
// 		[OBJ_PLANE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_COLOUR, true}},
// 		[OBJ_SPHERE] = {{PROP_POSITION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
// 		[OBJ_CYLINDER] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_DIMENSION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
// 		[OBJ_CONE] = {{PROP_POSITION, true}, {PROP_DIRECTION, true}, {PROP_DIMENSION, true}, {PROP_COLOUR, true}},
// 		[OBJ_TRIANGLE] = {{PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_POSITION, true}, {PROP_COLOUR, true}}
// 	};

// 	static char	*mat_keys[] = {
// 		[MAT_REFLECT] = "refl",
// 		[MAT_REFRACT] = "refr",
// 		[MAT_IDX_REFRACT] = "idx_refr",
// 		[MAT_SPECULAR] = "spec",
// 		[MAT_SHINE] = "shine",
// 		[MAT_CHECKER] = "checker",
// 		[MAT_IMG] = "image",
// 	};

// 	while (1)
// 	{
// 		//parse mandatory params specified in prop_format array
// 		// break ;
// 	}

// 	size_t	i;

// 	while (line)
// 	{
// 		i = 0;
// 		while (condition)
// 		{
// 			/* code */
// 		}
		
// 		ft_strncmp()
// 		//search for = in word
// 		//search the left word for an existing key
// 		//if match, search
// 		(*line)++;
// 	}
	
// 	//Should perhaps return the material object filled with its proper values
// }

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
			*object = (t_object){0};
			object->type = i;
			assets[i].shape_builder(object, line);
			assets[i].quantity++;
			return ;
		}
		i++;
	}
	put_err("Unidentified asset type");
}

//Add *splited_line[] and *line to glob object so it can be reached and freed anywhere
void	parse_rtconfig(char *path) 
{
	int		fd;
	size_t	index;
	t_parser	*parser;

	parser = parser_data();
	printf("Path %s of size %zu\n", path, ft_strlen(path));
	if (ft_strlen(path) < 3
		|| !ft_strnstr(&path[ft_strlen(path) - 3], ".rt", 3))
			put_err("Invalid file name");
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (put_err("Failled to open path"));
	while ((parser->line = get_next_line(fd)))
	{
		if (*parser->line == '\n') {
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
	ft_lstclear(&g_scene(0)->objects, clear_content);
}