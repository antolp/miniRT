/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 04:41:44 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//returns the sum of vectors a and b
//geometrically, this combines both directions into one.
t_vec3	vec_add(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x + b.x, a.y + b.y, a.z + b.z});
}

//returns the difference between vectors a and b
//each component of b is subtracted from a
//the resulting vector points from b to a in 3D space
t_vec3	vec_sub(t_vec3 a, t_vec3 b)
{
	return ((t_vec3){a.x - b.x, a.y - b.y, a.z - b.z});
}

//scales vector v by a scalar value
//changes the vector's length but not its direction
//(unless the scalar is negative)
t_vec3	vec_mul(t_vec3 v, double scalar)
{
	return ((t_vec3){v.x * scalar, v.y * scalar, v.z * scalar});
}

//length of vector using 3D Pythagorean theorem
double	vec_length(t_vec3 v)
{
	return (sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
}

//performance friendly
//useful when comparing distances rather than getting the actual length
double	vec_length_squared(t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}
