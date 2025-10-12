/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/12 18:28:52 by epinaud           #+#    #+#             */
/*   Updated: 2025/10/12 18:30:56 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	PARSER_H
# define PARSER_H
#include "rt.h"

// typedef enum e_objects_props
// {
// 	PROP_POSITION = 0,
// 	PROP_DIRECTION = 1 << 0,
// 	PROP_COLOUR = 1 << 1,
// 	PROP_RATIO = 1 << 2,
// 	PROP_BRIGHTNESS = 1 << 3,
// 	PROP_PATH = 1 << 4,
// 	PROP_DIAMETER = 1 << 5,
// 	PROP_FOV = 1 << 6,
// 	PROP_AXIS = 1 << 7
// }	t_object_props;

typedef enum e_objects_props
{
	PROP_POSITION,
	PROP_DIRECTION,
	PROP_COLOUR,
	PROP_RATIO,
	PROP_BRIGHTNESS,
	PROP_PATH,
	PROP_DIMENSION,
	PROP_FOV,
	PROP_AXIS
}	t_object_props;

void	parse_rtconfig(char *path);
void	put_err(char *msg);

#endif
