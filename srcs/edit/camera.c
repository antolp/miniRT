/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:29:16 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:31:27 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	handle_camera_yaw(int keycode, t_cli_var *var)
{
	if (keycode == XK_q || keycode == XK_k)
	{
		printf("Q pressed: Rotate camera left (yaw)\n");
		var->scene->camera.direction = rotate_yaw(
				var->scene->camera.direction, -var->scale);
		var->r->has_drawn_realtime = false;
		return (1);
	}
	if (keycode == XK_e || keycode == XK_semicolon)
	{
		printf("E pressed: Rotate camera right (yaw)\n");
		var->scene->camera.direction = rotate_yaw(
				var->scene->camera.direction, var->scale);
		var->r->has_drawn_realtime = false;
		return (1);
	}
	return (0);
}

int	handle_camera_pitch(int keycode, t_cli_var *var)
{
	if (keycode == XK_o)
	{
		printf("O pressed: Rotate camera upward (pitch)\n");
		var->scene->camera.direction = rotate_pitch(
				var->scene->camera.direction, var->scale);
		var->r->has_drawn_realtime = false;
		return (1);
	}
	if (keycode == XK_l)
	{
		printf("L pressed: Rotate camera downward (pitch)\n");
		var->scene->camera.direction = rotate_pitch(
				var->scene->camera.direction, -var->scale);
		var->r->has_drawn_realtime = false;
		return (1);
	}
	return (0);
}

int	handle_camera_fov_in(int keycode, t_cli_var *var)
{
	if (keycode == XK_z && var->scene->camera.fov > 10)
	{
		printf("Z pressed: Zoom in (fov : %f)\n",
			var->scene->camera.fov - 5);
		if (var->scene->camera.fov > 10)
			var->scene->camera.fov -= 5;
		else
			printf("Minimum FOV reached\n");
		var->r->has_drawn_realtime = false;
		return (1);
	}
	return (0);
}

int	handle_camera_fov_out(int keycode, t_cli_var *var)
{
	if (keycode == XK_x && var->scene->camera.fov < 130)
	{
		printf("X pressed: Zoom out (fov : %f)\n",
			var->scene->camera.fov + 5);
		if (var->scene->camera.fov < 130)
			var->scene->camera.fov += 5;
		else
			printf("Maximum FOV reached\n");
		var->r->has_drawn_realtime = false;
		return (1);
	}
	return (0);
}
