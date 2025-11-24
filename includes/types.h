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

# include <stdbool.h>

//This header is used to declare small types used
//throughout the code of the program

//2 dimensional vector/coordinates
typedef struct s_vec2
{
	double	x;
	double	y;
}	t_vec2;

//3 dimensional vector/coordinates
typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

//color described by red, blue and green components as scalar in [0..255]
typedef struct s_color
{
	int		r;
	int		g;
	int		b;
}	t_color;

//ray defined by its orgin, its direction and its depth in the recursive logic
typedef struct s_ray
{
	t_vec3	origin;
	t_vec3	direction;
	int		depth;
}	t_ray;

typedef enum e_object_type
{
	OBJ_UNKNOWN,
	OBJ_PLANE,
	OBJ_SPHERE,
	OBJ_CYLINDER,
	OBJ_CONE,
	OBJ_TRIANGLE
}	t_object_type;

typedef enum e_texture_type
{
	TEXTURE_NONE,
	TEXTURE_CHECKER,
	TEXTURE_IMAGE
}	t_texture_type;

//below, helper structs for math, ray-intersection and shading pipeline logic
typedef struct s_quad
{
	double	a;
	double	b;
	double	c;
	double	d;
	double	sqrt_d;
	double	t0;
	double	t1;
}	t_quad;

typedef struct s_cap_vars
{
	t_vec3	center;
	t_vec3	normal;
	double	denom;
	double	t;
	double	best_t;
	t_vec3	p;
	t_vec3	best_p;
	int		i;
	bool	found;
}	t_cap_vars;

typedef struct s_cyl_side_vars
{
	t_vec3	c_to_o;
	t_vec3	d;
	t_vec3	axis;
	t_vec3	d_perp;
	t_vec3	cto_perp;
}	t_cyl_side_vars;

typedef struct s_cone_side_vars
{
	t_vec3	co;
	t_vec3	d;
	double	cos2;
	double	dv;
	double	cov;
}	t_cone_side_vars;

typedef struct s_cone_cap_vars
{
	t_vec3	center;
	t_vec3	normal;
	double	radius;
	double	denom;
	double	t;
}	t_cone_cap_vars;

typedef struct s_tri_vars
{
	t_vec3	edge1;
	t_vec3	edge2;
	t_vec3	pvec;
	t_vec3	tvec;
	t_vec3	qvec;
	double	det;
	double	inv_det;
	double	u;
	double	v;
	double	tcand;
}	t_tri_vars;

//forward declarations for helper structs used below
struct	s_cylinder;
struct	s_cone;
struct	s_object;
struct	s_material;
struct	s_texture_image;
struct	s_renderer;
struct	s_scene;
struct	s_camera_basis;

typedef struct s_cyl_uv_vars
{
	struct s_cylinder	*cyl;
	t_vec3				axis;
	t_vec3				tangent;
	t_vec3				bitangent;
	t_vec3				d;
	t_vec3				rv;
	double				h;
	double				rlen;
	double				eps_h;
	double				eps_r;
}	t_cyl_uv_vars;

//d -> distance from hit_p to apex
//s -> axial coord from apex (dot(d,A))
//base_c -> base center = apex + A*height
//r_base -> base radius = height * tan(angle)
typedef struct s_cone_uv_vars
{
	struct s_cone		*cone;
	t_vec3				axis;
	t_vec3				tangent;
	t_vec3				bitangent;
	t_vec3				hit_p;
	t_vec3				d;
	double				s;
	double				eps_h;
	double				eps_r;
	t_vec3				base_c;
	double				r_base;
	t_vec3				w;
	double				rlen;
	double				r_here;
}	t_cone_uv_vars;

typedef struct s_bary_out
{
	double	b0;
	double	b1;
	double	b2;
}	t_bary_out;

typedef struct s_bump_var
{
	struct s_object			*obj;
	struct s_material		*mat;
	struct s_texture_image	*img;
	t_vec3					hit_p;
	t_vec2					uv;
	double					du;
	double					dv;
	double					dhdu;
	double					dhdv;
	t_vec3					normal;
	t_vec3					tangent;
	t_vec3					bitangent;
}	t_bump_var;

typedef struct s_cli_var
{
	struct s_renderer		*r;
	struct s_scene			*scene;
	t_vec3					world_up;
	t_vec3					right;
	double					scale;
}	t_cli_var;

typedef struct s_supersample_var
{
	struct s_camera_basis	*cb;
	int						samples;
	double					offset;
}	t_supersample_var;

#endif
