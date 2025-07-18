/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

//This header is used to declare small types used
//throughout the code of the program

//3 dimensional vector
typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

//color described by red, blue and green components
typedef struct s_color
{
	int	r;
	int	g;
	int	b;
}	t_color;

//ray defined by its orgin and its direction
//makes maths easier than with an origine-defined normalized vec3
typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

//each object types
typedef enum e_object_type
{
	OBJ_UNKNOWN,
	OBJ_PLANE,
	OBJ_SPHERE,
	OBJ_CYLINDER,
	OBJ_CONE,
	OBJ_TRIANGLE
}	t_object_type;

//each texture types
typedef enum e_texture_type
{
	TEXTURE_NONE,
	TEXTURE_IMAGE,
	TEXTURE_CHECKERBOARD,
	TEXTURE_BUMPMAP
}	t_texture_type;

#endif
