/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaner.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/25 01:23:25 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/26 18:54:13 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static int	destroy_mlx(t_renderer *renderer)
{
	if (renderer->win)
	{
		mlx_clear_window(renderer->mlx, renderer->win);
		mlx_destroy_window(renderer->mlx, renderer->win);
	}
	if (renderer->img.img)
		mlx_destroy_image(renderer->mlx, renderer->img.img);
	mlx_destroy_display(renderer->mlx);
	free(renderer->mlx);
	renderer->mlx = NULL;
	return (0);
}


//destroy mlx image and frees texture pointer
void	destroy_texture(void *mlx, t_texture *tex)
{
	t_tex_img	*img;

	if (tex == NULL)
		return ;
	if (tex->data != NULL)
	{
		img = (t_tex_img *)tex->data;
		if (img->mlx_img && mlx && tex->type == TEXTURE_IMAGE)
			mlx_destroy_image(mlx, img->mlx_img);
		free(img);
	}
}

static void	clear_content(t_list *elm)
{
	t_object	*obj;

	if (!elm->content)
		return ;
	obj = elm->content;
	if (obj->material.texture.data)
		destroy_texture(g_renderer(0)->mlx, &obj->material.texture);
	if (obj->material.bump_maps.data)
		destroy_texture(g_renderer(0)->mlx, &obj->material.bump_maps);
	if (obj->shape)
		free(obj->shape);
	if (elm->content)
		free(elm->content);
}

static void	clear_lights(t_list *elm)
{
	if (!elm->content)
		return ;
	if (elm->content)
		free(elm->content);
}

void	clear_minirt(void)
{
	ft_lstclear(&g_scene(0)->objects, clear_content);
	ft_lstclear(&g_scene(0)->lights, clear_lights);
	if (parser_data()->line)
		free(parser_data()->line);
	if (parser_data()->word_arr)
		ft_free_dynarr(parser_data()->word_arr);
	destroy_mlx(g_renderer(0));
}
