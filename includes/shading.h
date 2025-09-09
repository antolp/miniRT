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

# include "types.h"

/* Feature toggles (bitmask) */
# define FLAG_SHADOW				(1u << 0)
# define FLAG_TRANSPARENT_SHADOW	(1u << 1)
# define FLAG_SPECULAR				(1u << 2)
# define FLAG_REFLECTION			(1u << 3)
# define FLAG_REFRACTION			(1u << 4)
# define FLAG_FRESNEL				(1u << 5)
# define TEXTURE					(1u << 6)
# define BUMP_MAP					(1u << 7)

// # define DEFAULT_FLAGS ( \
// 	FLAG_SPECULAR | FLAG_REFLECTION | FLAG_REFRACTION | \
// 	FLAG_SHADOWS  | FLAG_TONEMAP )

# define DEFAULT_FLAGS ( \
	FLAG_SHADOW | FLAG_TRANSPARENT_SHADOW | FLAG_SPECULAR | \
	FLAG_REFLECTION | FLAG_REFRACTION | FLAG_FRESNEL | \
	TEXTURE | BUMP_MAP )




#endif
