/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:10:46 by epinaud           #+#    #+#             */
/*   Updated: 2025/10/03 12:24:36 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"
// #include "stdio.h"
typedef struct s_asset {
	char			*type_id;
	t_object_type	params[5];
}	t_asset;

//
bool	parse_vec3(char *val, t_vec3 *vec)
{
	float	num;
	size_t	ret_atof;

	//parse x
	num = 0;
	ret_atof = ft_atof(val, &num);
	if (!ret_atof || val[ret_atof] != ',')
		printf("Num: %f [vec3] Invalid data : Unexpected value or Missing coordinate\n", num);
	printf("Num: %f \n", num);
	val += ret_atof + 1;
	vec->x = num;
	num = 0;
	//parse y
	ret_atof = ft_atof(val, &num);
	if (!ret_atof || val[ret_atof] != ',')
		printf("[vec3] Invalid data : Unexpected value or Missing coordinate\n");
	val += ret_atof + 1;
	vec->y = num;
	num = 0;
	//parse z
	ret_atof = ft_atof(val, &num);
	if (!ret_atof)
		printf("[vec3] Invalid data : Unexpected value or Missing coordinate\n");
	vec->z = num;
	return (1);
}

//Charged to assert the integrity of given property value
bool	check_type(size_t prop, char **value) {
	float	sum;

	sum = 0;
	ft_atof("   2546.5874lll", &sum);
	printf("Atof result : %f\n", sum);
	if (prop == PROP_POSITION)
	{

	}
	else if (prop == PROP_DIRECTION)
	{
		// ft_atoi();
		// if (**value != '.')

	}
	else if (prop == PROP_AXIS)
	{
		// ft_atoi();
		// if (**value != '.')

	}
	return (1);
}

void	check_params(t_asset *object, char **line) {
	size_t	i = 0;
	// static t_asset asset[] = {
	// 	{.type_id = "Test", .params = {"param1", "param2", 0}}
	// };
	// asset->type_id = "Changed litteral";
	// printf("Asset 0 name : %s, Param 1 : %s Param 2 %s \n", asset->type_id, asset->params[0], asset->params[1]);
	while (**line != ' ')
	{
	
	}
	
}

void	parse_object(char **line)
{
	static const size_t	asset_count = 10;
	size_t	i;
	static char	*assets[] = {
		[0] = "A", [1] = "C", [2] = "L", [3] = "BG", 
		[4] = "SB",	[5] = "sp", [6] = "pl",
		[7] = "cy", [8] = "co", [9] = "tr"
	};
	//Element
	//Priority
	//Unicity


	while (**line == ' ')
		*line++;
	i = 0;
	
	while (assets[i])
	{
		if (i >= asset_count)
			printf("PUT ERROR : unidentified asset type\n");
		if (ft_strnstr(*line, assets[i], ft_strlen(assets[i])))
			break ;
		i++;
	}
	*line += ft_strlen(assets[i]);
	
}

void	parse_rt(char *path) 
{
	// size_t	i;
	int		fd;
	char	*line = "1,4,6.78";
	t_vec3	vec = {0};
	
	parse_vec3(line, &vec);
	printf("Vec x %f, y %f , z %f\n", vec.x, vec.y, vec.z);
	// check_type(PROP_AXIS, &line);
	return ;
	// check path integrity
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return /* error trigger */;
	while (line = get_next_line(fd))
	{
		while (*line)
			parse_object(&line);
	}

}