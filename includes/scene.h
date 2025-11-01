/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCENE_H
# define SCENE_H

//This header is used to declare scene, lights, camera, renderer
//and MLX related structs

# include <stdbool.h>
# include "types.h"
# include "objects.h"
# include "libft.h"

typedef struct s_light
{
	t_vec3	position;
	double	intensity;
	t_color	color;
}	t_light;

//camera context infos
typedef struct s_camera_basis
{
	t_vec3	forward;
	t_vec3	right;
	t_vec3	up;
	double	aspect_ratio;
	double	scale;
}	t_camera_basis;

typedef struct s_camera
{
	t_vec3	position;
	t_vec3	direction;
	double	fov;
}	t_camera;

typedef struct s_scene
{
	t_camera	camera;
	t_list		*objects;		//list of t_object*
	t_list		*lights;		//list of t_light*
	t_color		ambient_color;
	float		ambient_ratio;
	bool		is_rendering;
	t_color		background_color;
	t_texture	skybox;
}	t_scene;

#endif
