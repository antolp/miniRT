/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:28:52 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/26 19:04:22 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H
# include "rt.h"

# define ERR_CHECKER_FORMAT "Checkerboard : Missing separator ; or value\n \
	checker=R,G,B;R,G,B;scaleU;scaleV"

typedef struct s_parser
{
	char	*line;
	char	**word_arr;
}	t_parser;

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

typedef struct s_property_rules
{
	t_property_type	type;
	double			min;
	double			max;
}	t_property_rules;

typedef struct s_asset_format
{
	char	*type;
	size_t	quantity;
	size_t	qtmin;
	size_t	qtmax;
	void	(*shape_builder)();
	size_t	type_enum;
}	t_asset_format;

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

typedef struct s_material_dispatcher
{
	char				*key;
	void				*val;
	t_property_type		processing_type;
}	t_material_dispatcher;

void		parse_rtconfig(char *path);
void		put_err(char *msg);
t_parser	*parser_data(void);

//Parser modules
bool		set_property(size_t type, void *dst, char *line);
void		parse_mats(t_material *mat, char **line);

//Utils
void			check_range(double val, t_property_rules range);
t_list			*add_content(void *content);
t_asset_format	*get_asset_rules(char *type, t_object_type enm_type);

//Shape builders
void		build_sphere(t_object *obj, char **line);
void		build_plane(t_object *obj, char **line);
void		build_cylinder(t_object *obj, char **line);
void		build_cone(t_object *obj, char **line);
void		build_triangle(t_object *obj, char **line);
void		build_ambiant_light(t_scene *set, char **line);
void		build_camera(t_scene *set, char **line);
void		build_light(t_scene *set, char **line);
void		build_background(t_scene *set, char **line);
void		build_skybox(t_scene *set, char **line);

#endif
