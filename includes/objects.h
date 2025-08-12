/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OBJECTS_H
# define OBJECTS_H

# include "types.h"

//This header is used to declare all objects and their components
//(textures, material properties)

typedef struct s_checkerboard
{
	t_color	color1;
	t_color	color2;
	double	scale;
}	t_checkerboard;

typedef struct s_texture
{
	t_texture_type	type;
	void			*data; // texture image or procedural config
}	t_texture;

typedef struct s_material
{
	t_color			base_color;
	double			reflectivity;
	double			shininess;
	double			specular_coef;
	double			ior;
	t_texture		texture;
}	t_material;

typedef struct s_sphere
{
	t_vec3		center;
	double		radius;
}	t_sphere;

typedef struct s_plane
{
	t_vec3		point;
	t_vec3		normal;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3		center;
	t_vec3		axis;
	double		radius;
	double		height;
}	t_cylinder;

typedef struct s_cone
{
	t_vec3		apex;
	t_vec3		axis;
	double		angle;
	double		height;
}	t_cone;

//vertices must be defined in anti-clockwise order from Camera from view !!
//(or, won't be facing camera, no shading)
typedef struct s_triangle
{
	t_vec3		p0;
	t_vec3		p1;
	t_vec3		p2;
}	t_triangle;


//polymorphic wrapper struct for objects
typedef struct s_object
{
	t_object_type	type;
	void			*shape; // Cast to specific object struct
	t_material		material;
	bool			(*intersect)(struct s_object *, t_ray *, double *t);
	void			(*get_normal)(struct s_object *obj, t_vec3 *hit,
			t_vec3 *out_normal);
}	t_object;

#endif
