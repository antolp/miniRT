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

# define FLAG_COUNT 9
# include "types.h"

//Feature toggles
# define FLAG_SHADOW              0x00000001u
# define FLAG_TRANSPARENT_SHADOW  0x00000002u
# define FLAG_DIFFUSE             0x00000004u
# define FLAG_SPECULAR            0x00000008u
# define FLAG_REFLECTION          0x00000010u
# define FLAG_REFRACTION          0x00000020u
# define FLAG_FRESNEL             0x00000040u
# define FLAG_TEXTURE             0x00000080u
# define FLAG_BUMP_MAP            0x00000100u

# define DEFAULT_FLAGS 0x000001FFu

#endif