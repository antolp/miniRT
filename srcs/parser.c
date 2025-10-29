/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:10:46 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/01 00:07:03 by epinaud          ###   ########.fr       */
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
	//free image path in material
		//if (obj->material.texture.)
	if (obj->material.texture.data)
		free(obj->material.texture.data);

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
	int	ret_atof;
	
	ret_atof = 0;
	while (*valset) 
	{
		if (type == PROP_COLOUR)
			ret_atof = ft_atoi2(line, *valset);
		else if (type == PROP_POSITION || type == PROP_DIRECTION)
			ret_atof = ft_atof(line, *valset);

		if (type == PROP_COLOUR)
			printf("Sum is now %d with len of %d\n", **(int **)valset, ret_atof);
		else if (type == PROP_POSITION || type == PROP_DIRECTION)
			printf("Sum is now %f with len of %d\n", **(double **)valset, ret_atof);

		if (ret_atof < 1 || !ft_strchr(";,\n\0", line[ret_atof]))
			put_err("Invalid data : Unexpected or Missing value\n");
		line += ret_atof + 1;
		valset++;
	}
}

//Count and check object quantity
void	check_range(double val, t_property_rules range)
{
	// if (in_array(prop, chkRange, nb_elems(chkRange, sizeof(chkRange)))) {
	if ( val < range.min || val > range.max )
	{
		ft_dprintf(STDERR_FILENO, "Expecting value between [%f] and [%f]", range.min, range.max);
		put_err("Parameter has out of range value");
	}
}

//!!!Now rendered obsolete by the dynamic material dispatcher
// bool	set_mat(size_t type, t_material *mat, char *line, void *dst)
// {
// 	if (type >= MAT_REFLECT && type <= MAT_SHINE)
// 		set_property(PROP_SIZE, dst, line);	
// 	else if (type == MAT_IMG)
// 		set_property(PROP_PATH, dst, line);
// 	else if (type == MAT_CHECKER)
// 	{
		
// 	}
// 	//check values range
// }

//Charged to assert the integrity of given property value
bool	set_property(size_t type, void *dst, char *line)
{
	t_morph	un;
	float	sum;
	static t_property_rules	props[] = {
		[PROP_POSITION] = {PROP_POSITION, FLT_MIN, FLT_MAX},
		[PROP_DIRECTION] = {PROP_DIRECTION, -1, 1},
		[PROP_COLOUR] = {PROP_COLOUR, 0, 255},
		[PROP_RATIO] = {PROP_RATIO, 0.0, 1.0},
		[PROP_ANGLE] = {PROP_ANGLE, 0, 180},
		[PROP_SIZE] = {PROP_SIZE, FLT_MIN, FLT_MAX},
		[PROP_PATH] = {PROP_PATH, 0, 0}
		// {PROP_VALUE, {PROP_VALUE, FLT_MIN, FLT_MAX}
	};
	
	
	printf(" >>> Calling setproperty with line %s\n", line);
	if (!line)
		put_err("Invalid data : missing parameter");
	sum = 0;
	if (type == PROP_POSITION || type == PROP_DIRECTION)
	{
		// t_vec3	*vec = dst;
		un.vec = dst;

		parse_valset(line, (void *[]){&un.vec->x, &un.vec->y, &un.vec->z, 0}, PROP_DIRECTION);
		// printf("String after vec3 parsing: %s\n", line);
		if (PROP_DIRECTION && (!un.vec->x && !un.vec->y && !un.vec->z))
			put_err("Invalid vector dimensions : one axis should not be 0");
		printf("Valset coordinates : %f,%f,%f \n",
		un.vec->x, un.vec->y, un.vec->z);
	}
	else if (type == PROP_COLOUR)
		parse_valset(line, (void *[]){&((t_color *)dst)->r, &((t_color *)dst)->g,
			&((t_color *)dst)->b, 0}, PROP_COLOUR);
	else if (type == PROP_PATH)
	{
		char	*path;

		if (ft_strchr(line, ' '))
			path = ft_substr(line, 0, ft_strchr(line, ' ') - line);
		else
			path = ft_substr(line, 0, ft_strlen(line));
		if (ft_strlen(path) < 4
			|| !ft_strnstr(path + ft_strlen(path) - 4, ".xpm", 4))
			put_err("Invalid file name: expecting *.xpm");
		((t_texture *)dst)->type = TEXTURE_IMAGE;
		((t_texture *)dst)->data = load_xpm_image(g_renderer(NULL)->mlx, path);
		//check proper xmp img loading
		free(path);
	}
	else if (type == PROP_CHECKER)
	{
		t_checkerboard	*cb;

		cb = malloc(sizeof(t_checkerboard));
		if (!cb)
			return (false);
		*cb = (t_checkerboard){0};
		parse_valset(line, (void *[]){&cb->color1.r, &cb->color1.g, &cb->color1.b, 0}, PROP_COLOUR);
		line = ft_strchr(line, ';') + 1;
		if (!line)
			put_err("Checkerboard : Insufficient parameter count");
		parse_valset(line, (void *[]){&cb->color2.r, &cb->color2.g, &cb->color2.b, 0}, PROP_COLOUR);
		line = ft_strchr(line, ';') + 1;
		if (!line)
			put_err("Checkerboard : Insufficient parameter count");
		printf("line is now %s\n", line);
		set_property(PROP_SIZE, &cb->scale_u, line);		
		line = ft_strchr(line, ';') + 1;
		if (!line)
			put_err("Checkerboard : Insufficient parameter count\n Expecting: checker=R,G,B;R,G,B;scaleU;scaleV");
		set_property(PROP_SIZE, &cb->scale_u, line);
		*(t_texture *)dst = (t_texture){.type = TEXTURE_CHECKER, .data = cb};
	}
	else {
		if (ft_atof(line, (double *)dst) < 1)
			put_err("[Atof-Atoi2] : Overflow or parsing error");
	}
	return (1);
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

t_material_dispatcher	*build_dispatcher(void **vals)
{
	static t_material_dispatcher disp[] = {
	[MAT_REFLECT] = {.key = "refl", .processing_type = PROP_SIZE},
	[MAT_REFRACT] = {.key = "refr", .processing_type = PROP_SIZE},
	[MAT_IDX_REFRACT] = {.key = "idx_refr", .processing_type = PROP_SIZE},
	[MAT_SPECULAR] = {.key = "spec", .processing_type = PROP_SIZE},
	[MAT_SHINE] = {.key = "shine", .processing_type = PROP_SIZE},
	[MAT_CHECKER] = {.key = "checker", .processing_type = PROP_CHECKER},
	[MAT_IMG] = {.key = "image", .processing_type = PROP_PATH}, NULL};
	size_t	i;

	i = 0;
	while (vals[i])
	{
		disp[i].val = vals[i];
		i++;
	}
	return (disp);
}

void	parse_mats(t_material *mat, char **line)
{
	size_t	len;
	size_t	i;
	t_material_dispatcher	*mat_disp;

	mat_disp = build_dispatcher((void *[]){[MAT_REFLECT] = &mat->reflectivity, 
		[MAT_REFRACT] = &mat->refractivity, [MAT_IDX_REFRACT] = &mat->ior,
		[MAT_SPECULAR] = &mat->specular_strength, [MAT_SHINE] = &mat->shininess,
		[MAT_CHECKER] = &mat->texture, [MAT_IMG] = &mat->texture, NULL});
	while (*line && **line != '\n')
	{
		i = 0;
		while (mat_disp[i].key)
		{
			printf("Checking mat:  %s in line %s\n", mat_disp[i].key, *line);
			len = ft_strlen(mat_disp[i].key);
			
			if (!ft_strncmp(mat_disp[i].key, *line, len) && ft_strchr("=:", (*line)[len])) {
				printf("Found the right mat <3\nRemaining val %s\n", *line + len + 1);
				set_property(mat_disp[i].processing_type, mat_disp[i].val, *line + len + 1);
			}
			i++;
		}
		line++;
	}
	if (!mat->shininess)
	mat->shininess = 32;
	if (!mat->ior)
	{
		if (mat->reflectivity)
			mat->ior = 1.3;
		else if (mat->refractivity)
			mat->ior = 1.02;
		else
			mat->ior = 1.00;
	}
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
		// printf("asset type is %s\n", assets[i].type);
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
	ft_lstclear(&g_scene(0)->objects, clear_content);
}