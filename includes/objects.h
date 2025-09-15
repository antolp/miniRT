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

//see uv_triangle.c
typedef enum e_tri_uv_mode
{
	TRI_UV_ORTHONORMAL,   /* no deformation (your current frame U,V) */
	TRI_UV_FIT,           /* p0->(0,0), p1->(1,0), p2->(0,1) */
	TRI_UV_FIT_OPPOSITE   /* complementary half of the square */
}	t_tri_uv_mode;

//like so_long, XPM ONLY
typedef struct s_texture_image {
    void   *mlx_img;      // handle from mlx_xpm_file_to_image
    char   *addr;         // from mlx_get_data_addr
    int     bpp;
    int     line_len;
    int     endian;
    int     width;
    int     height;
    int     bgra;         // 1 if BGRA ordering, happens sometimes
} t_texture_image;


//parametric checker texture
//immensely useful when debugging
typedef struct s_checkerboard
{
    t_color color1;
    t_color color2;
    double  scale_u;
    double  scale_v;
}   t_checkerboard;

//appart from checkerboard, pseudo-random noise could be a cool proc texture
typedef struct s_texture
{
	t_texture_type	type;
	void			*data; // texture image or procedural config
}	t_texture;

//reflectance and refractance mutually exclusive because it's a pain in the ass
typedef struct s_material
{
	t_color		base_color;
	double		shininess;			// [~10, ~300] specular exponent (Blinn-Phong)
	double		specular_strength;	// [0, 1 (or a bit more)], intensity of highlight
	double		reflectivity;		// [0, 1], used only if reflective
	double		ior;				// index of refraction, different roles whether reflective or refractive.
	double		refractivity;		// [0, 1] if more than 0, transparent and nothing else
	t_texture	texture;
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
	t_vec3			p0;
	t_vec3			p1;
	t_vec3			p2;
	t_tri_uv_mode	uv_mode;
}	t_triangle;

//polymorphic wrapper struct for objects
typedef struct s_object
{
	t_object_type	type;
	void			*shape;
	t_material		material;
	bool			(*intersect)(struct s_object *, t_ray *, double *t);
	void			(*get_normal)(struct s_object *obj, t_vec3 *hit,
			t_vec3 *out_normal);
	bool			(*get_uv)(struct s_object *, t_vec3 *hit, t_vec2 *out_uv);
}	t_object;

#endif
