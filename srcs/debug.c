/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anle-pag <anle-pag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 20:10:12 by anle-pag          #+#    #+#             */
/*   Updated: 2025/07/15 16:47:45 by anle-pag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

//A SUPPR
//ou trouver un moyen de print des float sans printf, jsp
void	print_vec3(char *s, t_vec3 v)
{
	if (ft_strlen(s) > 5)
		printf("%s :\t(%.2f, %.2f, %.2f)\n", s, v.x, v.y, v.z);
	else
		printf("%s :\t\t(%.2f, %.2f, %.2f)\n", s, v.x, v.y, v.z);
}

void	print_color(char *s, t_color c)
{
	if (ft_strlen(s) > 5)
		ft_printf("%s :\t(R: %d, G: %d, B: %d)\n", s, c.r, c.g, c.b);
	else
		ft_printf("%s :\t\t(R: %d, G: %d, B: %d)\n", s, c.r, c.g, c.b);

}