/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   materials.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 23:23:39 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/28 17:11:35 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

static t_material_dispatcher	*build_dispatcher(void **vals)
{
	static t_material_dispatcher	disp[] = {
	[MAT_REFLECT] = {.key = "refl", .processing_type = PROP_SIZE},
	[MAT_REFRACT] = {.key = "refr", .processing_type = PROP_SIZE},
	[MAT_IDX_REFRACT] = {.key = "idx_refr", .processing_type = PROP_SIZE},
	[MAT_SPECULAR] = {.key = "spec", .processing_type = PROP_SIZE},
	[MAT_SHINE] = {.key = "shine", .processing_type = PROP_SIZE},
	[MAT_CHECKER] = {.key = "checker", .processing_type = PROP_CHECKER},
	[MAT_BUMP] = {.key = "bump", .processing_type = PROP_PATH},
	[MAT_IMG] = {.key = "image", .processing_type = PROP_PATH},
	[MAT_BUMP + 1] = {NULL}};
	size_t							i;

	i = 0;
	while (vals[i])
	{
		disp[i].val = vals[i];
		i++;
	}
	return (disp);
}

static void	default_mats(t_material *mat)
{
	if (!mat->shininess)
		mat->shininess = 32;
	if (!mat->ior)
	{
		if (mat->reflectivity)
			mat->ior = 1.3;
		else if (mat->refractivity)
			mat->ior = 1.02;
		else
			mat->ior = 1.00;
	}
	mat->bump_strength = 1.00;
}

//Assigns all ptrs of the provided instance of t_material
// to a dispatcher for their incoming automatic parsing
//Warning: nothing yet prevents a material to be parsed twice;
// left as such since values will just "atoi" convert over one another
void	parse_mats(t_material *mat, char **line)
{
	size_t					len;
	size_t					i;
	t_material_dispatcher	*disp;

	disp = build_dispatcher((void *[]){[MAT_REFLECT] = &mat->reflectivity,
		[MAT_REFRACT] = &mat->refractivity, [MAT_IDX_REFRACT] = &mat->ior,
		[MAT_SPECULAR] = &mat->specular_strength, [MAT_SHINE] = &mat->shininess,
		[MAT_CHECKER] = &mat->texture, [MAT_IMG] = &mat->texture,
		[MAT_BUMP] = &mat->bump_maps, NULL});
	while (*line && **line != '\n')
	{
		i = 0;
		while (disp[i].key)
		{
			len = ft_strlen(disp[i].key);
			if (!ft_strncmp(disp[i].key, *line, len)
				&& (*line)[len] && ft_strchr("=:", *(*line + len)))
				set_property(disp[i].processing_type, disp[i].val,
					*line + ++len);
			i++;
		}
		line++;
	}
	default_mats(mat);
}
