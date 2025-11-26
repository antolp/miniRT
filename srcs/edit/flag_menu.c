/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:29:37 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:29:37 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

void	print_flags_table(uint32_t mask)
{
	int			i;
	uint32_t	bit;

	printf("\n=== Shading Flags Menu ===\n");
	printf("Press 1-9 to toggle a shading component.\nPress 'm' to close.\n\n");
	i = 0;
	while (i < FLAG_COUNT)
	{
		bit = flag_get_bit(i);
		if ((mask & bit) != 0u)
			printf("[%d] %-20s : ON\n", i + 1, flag_get_name(i));
		else
			printf("[%d] %-20s : OFF\n", i + 1, flag_get_name(i));
		i++;
	}
	printf("\n");
	fflush(stdout);
}

int	handle_edit_menu_keys_wrapper(int keycode, t_renderer *r)
{
	if (r->mode == RENDER_EDIT_MENU)
	{
		handle_menu_keys(keycode, r);
		return (1);
	}
	return (0);
}

int	handle_open_flags_menu(int keycode, t_renderer *r)
{
	if (keycode == XK_m && r->mode == RENDER_EDIT)
	{
		r->mode = RENDER_EDIT_MENU;
		printf("Opened flags menu.\n");
		print_flags_table(*(uint32_t *)&r->shading_flag);
		return (1);
	}
	return (0);
}
