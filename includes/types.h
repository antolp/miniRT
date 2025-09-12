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

//2 dimensional vector/point
typedef struct s_vec2
{
	double	x;
	double	y;
}	t_vec2;

//3 dimensional vector/point
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
	int		depth;
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
	TEXTURE_CHECKER,
	TEXTURE_IMAGE
}	t_texture_type;

//struct for norm compliance on quadratic computation
typedef struct s_quad {
	double	a; 
	double	b; 
	double	c;
	double	d;
	double	sqrt_d;
	double	t0;
	double	t1;
}	t_quad;


//for each struct here, add comments to explain values

//same for cylinder cap and body intersection computation
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
	t_vec3	co;     // O - apex, apex concidered center of cone
	t_vec3	d;      // ray direction
	double	cos2;   // cos(angle)^2
	double	dv;     // dot(d, axis)
	double	cov;    // dot(co, axis)
}	t_cone_side_vars;

typedef struct s_cone_cap_vars
{
	t_vec3	center; // base center
	t_vec3	normal; // axis
	double	radius; // base radius = height * tan(angle)
	double	denom;  // dot(ray->direction, normal)
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

struct s_cylinder;

// Packs everything shared across the cylinder UV helpers
typedef struct s_cyl_uv_vars
{
	struct s_cylinder	*cyl;
	t_vec3				A;       // axis (normalized)
	t_vec3				T;       // tangent
	t_vec3				B;       // bitangent
	t_vec3				d;       // P - center (per hit)
	t_vec3				rv;      // radial vector
	double				h;       // axial coord from bottom
	double				rlen;    // |rv|
	double				eps_h;   // axial tolerance
	double				eps_r;   // radial tolerance
}	t_cyl_uv_vars;

struct t_cone;


typedef struct s_cone_uv_vars
{
	struct s_cone	*cone;
	t_vec3			A;        // axis (unit)
	t_vec3			T;        // tangent
	t_vec3			B;        // bitangent
	t_vec3			P;        // hit point
	t_vec3			d;        // P - apex
	double			s;        // axial coord from apex (dot(d,A))
	double			eps_h;    // axial epsilon
	double			eps_r;    // radial epsilon
	t_vec3			baseC;    // base center = apex + A*height
	double			r_base;   // base radius = height * tan(angle)
	t_vec3			w;        // off-axis vector at P
	double			rlen;     // |w|
	double			r_here;   // expected radius at s
}	t_cone_uv_vars;


#endif