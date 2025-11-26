/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 02:29:44 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 02:47:54 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

int	handle_mode_switch(int keycode, t_renderer *r)
{
	if ((keycode == XK_E || keycode == XK_e)
		&& r->mode != RENDER_EDIT)
	{
		r->mode = RENDER_EDIT;
		r->has_drawn_realtime = false;
		printf("Switched to EDIT mode\nPress H for help\n");
		return (1);
	}
	if ((keycode == XK_R || keycode == XK_r)
		&& r->mode != RENDER_HIGH_QUALITY)
	{
		if (r->mode != RENDER_EDIT_MENU)
		{
			r->mode = RENDER_HIGH_QUALITY;
			r->render_done = false;
			printf("Switched to RENDER mode\n");
		}
		return (1);
	}
	return (0);
}

int	handle_quit(int keycode)
{
	if (keycode == XK_Escape)
	{
		g_renderer(NULL)->should_quit = true;
		printf("Exiting...\n");
		exit(0);
	}
	return (0);
}

int	handle_supersample_toggle(int keycode, t_renderer *r)
{
	if (keycode == XK_u)
	{
		if (r->supersampled == false)
		{
			printf("supersample on.\n");
			r->supersampled = true;
		}
		else
		{
			printf("supersample off.\n");
			r->supersampled = false;
		}
		return (1);
	}
	return (0);
}

//two functions to cycle trough lists of obj/lights
//here because of norm
t_list	*editor_list_last(t_list *lst)
{
	if (lst == NULL)
		return (NULL);
	while (lst->next != NULL)
		lst = lst->next;
	return (lst);
}

t_list	*editor_list_prev_wrap(t_list *head, t_list *node)
{
	t_list	*prev;

	if (head == NULL || node == NULL)
		return (NULL);
	if (head == node)
		return (editor_list_last(head));
	prev = head;
	while (prev->next != NULL && prev->next != node)
		prev = prev->next;
	if (prev->next == node)
		return (prev);
	return (node);
}
