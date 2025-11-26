/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_flag.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:30:59 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

const char	*flag_get_name(int index)
{
	static const char	*names[FLAG_COUNT] = {
		"shadow",
		"transparent shadow",
		"diffuse",
		"specular",
		"reflection",
		"refraction",
		"fresnel",
		"texture",
		"bump map"
	};

	if (index < 0 || index >= FLAG_COUNT)
		return ("wrongflagindex");
	return (names[index]);
}

uint32_t	flag_get_bit(int index)
{
	static const uint32_t	bits[FLAG_COUNT] = {
		FLAG_SHADOW,
		FLAG_TRANSPARENT_SHADOW,
		FLAG_DIFFUSE,
		FLAG_SPECULAR,
		FLAG_REFLECTION,
		FLAG_REFRACTION,
		FLAG_FRESNEL,
		FLAG_TEXTURE,
		FLAG_BUMP_MAP
	};

	if (index < 0 || index >= FLAG_COUNT)
		return (0);
	return (bits[index]);
}

static int	key_to_index(int keycode)
{
	if (keycode == XK_1)
		return (0);
	else if (keycode == XK_2)
		return (1);
	else if (keycode == XK_3)
		return (2);
	else if (keycode == XK_4)
		return (3);
	else if (keycode == XK_5)
		return (4);
	else if (keycode == XK_6)
		return (5);
	else if (keycode == XK_7)
		return (6);
	else if (keycode == XK_8)
		return (7);
	else if (keycode == XK_9)
		return (8);
	return (-1);
}

static void	toggle_index(uint32_t *mask, int index)
{
	uint32_t	bit;

	if (index < 0)
		return ;
	bit = flag_get_bit(index);
	if ((*mask & bit) != 0u)
	{
		*mask &= ~bit;
		printf("%s off.\n", flag_get_name(index));
	}
	else
	{
		*mask |= bit;
		printf("%s on.\n", flag_get_name(index));
	}
	print_flags_table(*mask);
}

//handles keys while in the flag menu
void	handle_menu_keys(int keycode, t_renderer *r)
{
	int			index;
	uint32_t	*mask;

	if (keycode == XK_m || keycode == XK_M)
	{
		r->mode = RENDER_EDIT;
		printf("Closed flags menu. Back to edit.\n");
		return ;
	}
	if (keycode == XK_r || keycode == XK_R)
	{
		printf("Close the menu before rendering.\n");
		return ;
	}
	index = key_to_index(keycode);
	mask = (uint32_t *)&r->shading_flag;
	if (index >= 0)
		toggle_index(mask, index);
}
