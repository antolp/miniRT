/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHADING_H
# define SHADING_H

#define FLAG_COUNT 9
# include "types.h"

//Feature toggles
//this is maxed out
//because there's no more than 9 digits, in base 10
# define FLAG_SHADOW				((uint32_t)1 << 0)
# define FLAG_TRANSPARENT_SHADOW	((uint32_t)1 << 1)
# define FLAG_DIFFUSE				((uint32_t)1 << 2)
# define FLAG_SPECULAR				((uint32_t)1 << 3)
# define FLAG_REFLECTION			((uint32_t)1 << 4)
# define FLAG_REFRACTION			((uint32_t)1 << 5)
# define FLAG_FRESNEL				((uint32_t)1 << 6)
# define FLAG_TEXTURE				((uint32_t)1 << 7)
# define FLAG_BUMP_MAP				((uint32_t)1 << 8)

# define DEFAULT_FLAGS ( \
	FLAG_SHADOW | FLAG_TRANSPARENT_SHADOW | FLAG_DIFFUSE | \
	FLAG_SPECULAR | FLAG_REFLECTION | FLAG_REFRACTION | FLAG_FRESNEL | \
	FLAG_TEXTURE | FLAG_BUMP_MAP )

#endif

