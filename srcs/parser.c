/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 18:10:46 by epinaud           #+#    #+#             */
/*   Updated: 2025/09/29 16:25:32 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "scene.h"

void	parse_object(char **line)
{
	static const size_t	asset_count = 10;
	size_t	i;
	static char	*assets[] = {
		[0] = "A", [1] = "C", [2] = "L", [3] = "BG", 
		[4] = "SB",	[5] = "sp", [6] = "pl",
		[7] = "cy", [8] = "co", [9] = "tr"
	};

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
	char	*line;
	
	//check path integrity
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (/* error trigger */);
	while (line = get_next_line(fd))
	{
		while (*line)
			parse_object(&line);
	}

}