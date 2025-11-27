/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   props_scale.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:30:07 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/27 16:46:41 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	handle_props_scale_primary(int keycode, t_cli_var *var)
{
	double	factor;

	if (keycode != XK_z && keycode != XK_Z
		&& keycode != XK_x && keycode != XK_X)
		return (0);
	if (var->r->edit.target == EDIT_TARGET_LIGHT)
	{
		printf("Scaling not available for lights.\n");
		return (1);
	}
	if (var->r->edit.target != EDIT_TARGET_OBJECT)
		return (1);
	if (keycode == XK_z || keycode == XK_Z)
		factor = 0.9;
	else
		factor = 1.1;
	editor_scale_object_primary(var, factor);
	return (1);
}

int	handle_props_scale_secondary(int keycode, t_cli_var *var)
{
	double	factor;

	if (keycode != XK_c && keycode != XK_C
		&& keycode != XK_v && keycode != XK_V)
		return (0);
	if (var->r->edit.target == EDIT_TARGET_LIGHT)
	{
		printf("Scaling not available for lights.\n");
		return (1);
	}
	if (var->r->edit.target != EDIT_TARGET_OBJECT)
		return (1);
	if (keycode == XK_c || keycode == XK_C)
		factor = 0.9;
	else
		factor = 1.1;
	editor_scale_object_secondary(var, factor);
	return (1);
}
