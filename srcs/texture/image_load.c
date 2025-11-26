/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_load.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/11/26 15:26:35 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//allocates the image texture struct in heap
//	add explicit malloc error message
static t_texture_image	*tex_image_alloc(void)
{
	t_texture_image	*img;

	img = (t_texture_image *)malloc(sizeof(t_texture_image));
	if (img == NULL)
		return (NULL);
	img->mlx_img = NULL;
	img->addr = NULL;
	img->bpp = 0;
	img->line_len = 0;
	img->endian = 0;
	img->width = 0;
	img->height = 0;
	img->bgra = 1;
	return (img);
}

//loads the MLX image and fills width/height
static int	load_mlx_xpm(void *mlx, const char *path, t_texture_image *img)
{
	int	w;
	int	h;

	w = 0;
	h = 0;
	img->mlx_img = mlx_xpm_file_to_image(mlx, (char *)path, &w, &h);
	if (img->mlx_img == NULL)
		return (ft_dprintf(2, "mlx_xpm_file_to_image() failed, %s\n"), path, 0);
	img->width = w;
	img->height = h;
	return (1);
}

//binds pixel buffer, on failure destroys the MLX image
//not sure, but i think mlx_get_data_addr() returns a pointer to where the
//image already got allocated, and populate the rest of the given pointer with
//already computed and stored data
static int	map_image_buffer(void *mlx, t_texture_image *img)
{
	img->addr = mlx_get_data_addr(img->mlx_img,
			&img->bpp, &img->line_len, &img->endian);
	if (img->addr == NULL)
	{
		mlx_destroy_image(mlx, img->mlx_img);
		img->mlx_img = NULL;
		return (0);
	}
	return (1);
}

//bgra useless
//all function used are here :
//https://harm-smits.github.io/42docs/libs/minilibx/prototypes
t_texture_image	*load_xpm_image(void *mlx, const char *path)
{
	t_texture_image	*img;

	if (mlx == NULL || path == NULL)
		return (NULL);
	img = tex_image_alloc();
	if (img == NULL)
		return (NULL);
	if (!load_mlx_xpm(mlx, path, img))
	{
		ft_dprintf(2, "image %s couldn't be loaded. please check the path.\n", path);
		return (free(img), NULL);
	}
	if (!map_image_buffer(mlx, img))
	{
		ft_dprintf(2, "loaded image %s data couldn't be mapped.\n", path);
		return (free(img), NULL);
	}
	if (img->bpp == 32)
		img->bgra = 1;
	else
		img->bgra = 0;
	ft_dprintf(1, "loaded xpm: %s (%dx%d, bpp=%d, line=%d, endian=%d)\n",
		path, img->width, img->height, img->bpp, img->line_len, img->endian);
	return (img);
}

//destroy mlx image and frees texture pointer
void	destroy_texture(void *mlx, t_texture *tex)
{
	t_texture_image	*img;

	if (tex == NULL)
		return ;
	if (tex->data != NULL)
	{
		img = (t_texture_image *)tex->data;
		if (img->mlx_img && mlx && tex->type == TEXTURE_IMAGE)
			mlx_destroy_image(mlx, img->mlx_img);
		free(img);
	}
}
