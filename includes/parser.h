/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:28:52 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/20 12:46:53 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSER_H
# define PARSER_H
#include "rt.h"

typedef struct s_parser
{
	char	*line;
	char	**word_arr;
}	t_parser;

// typedef enum e_objects_props
// {
// 	PROP_POSITION = 0,
// 	PROP_DIRECTION = 1 << 0,
// 	PROP_COLOUR = 1 << 1,
// 	PROP_SIZE = 1 << 2,
// 	PROP_RATIO = 1 << 3,
// 	PROP_PATH = 1 << 4,
// 	PROP_DIAMETER = 1 << 5,
// 	PROP_ANGLE = 1 << 6,
// 	PROP_AXIS = 1 << 7
// }	t_object_props;

typedef enum e_property_type
{
	PROP_POSITION,
	PROP_DIRECTION,
	PROP_COLOUR,
	PROP_SIZE,
	PROP_RATIO,
	PROP_PATH,
	PROP_ANGLE,
	PROP_CHECKER
}	t_property_type;

typedef struct s_property_rules {
	t_property_type	type_id;
	double			min;
	double			max;
}	t_property_rules;

typedef struct s_asset_format {
	char	*type;
	size_t	qtmax;
	size_t	qtmin;
	size_t	quantity;
	void	(*shape_builder)(t_object *obj, char **line);
}	t_asset_format;

typedef union s_morph
{
	t_vec3	*vec;
	t_color	*col;
}	t_morph;

typedef enum e_material_keys
{
	MAT_REFLECT,
	MAT_REFRACT,
	MAT_IDX_REFRACT,
	MAT_SPECULAR,
	MAT_SHINE,
	MAT_CHECKER,
	MAT_IMG,
	MAT_BUMP
}	t_material_keys;

typedef struct s_material_dispatcher {
	char				*key;
	void				*val;
	t_property_type		processing_type;
}	t_material_dispatcher;

void	parse_rtconfig(char *path);
void	put_err(char *msg);


//Shape builders
t_list		*add_content(void *content);
void	build_sphere(t_object *obj, char **line);
void	build_plane(t_object *obj, char **line);
void	build_cylinder(t_object *obj, char **line);
void	build_cone(t_object *obj, char **line);
void	build_triangle(t_object *obj, char **line);
void	build_ambiant_light(t_scene *set, char **line);
void	build_camera(t_scene *set, char **line);
void	build_light(t_scene *set, char **line);
void	build_background(t_scene *set, char **line);
void	build_skybox(t_scene *set, char **line);
bool	set_property(size_t type, void *dst, char *line);
void	parse_mats(t_material *mat, char **line);

#endif
