/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 18:08:54 by epinaud           #+#    #+#             */
/*   Updated: 2025/11/28 18:17:24 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rt.h"

t_parser	*parser_data(void)
{
	static t_parser	s = (t_parser){0};

	return (&s);
}

void	put_err(char *msg)
{
	ft_dprintf(STDERR_FILENO, msg);
	ft_putchar_fd('\n', STDERR_FILENO);
	clear_minirt();
	exit(EXIT_FAILURE);
}

//Checks a given property against a range of values
//Cleaner print for future float / double handling in printf:
//ft_dprintf(STDERR_FILENO, "Expecting value for type %u
//between [%f] and [%f] for value %f\n",
//range.type, range.min, range.max, val);
void	check_range(double val, t_property_rules range)
{
	if (val < range.min || val > range.max)
	{
		ft_dprintf(STDERR_FILENO, "Out of range value, ");
		ft_dprintf(STDERR_FILENO, "expecting between [min] & [max]:\n");
		ft_dprintf(STDERR_FILENO, " [POSITION] — -FLT_MAX, FLT_MAX\n");
		ft_dprintf(STDERR_FILENO, " [DIRECTION] — -1, 1\n [COLOUR] — 0, 255\n");
		ft_dprintf(STDERR_FILENO, " [RATIO] — 0.0, 1.0\n [ANGLE] — 0, 180\n");
		ft_dprintf(STDERR_FILENO, " [SIZE] — 0, FLT_MAX\n [PATH] — MIN: 4");
		put_err("");
	}
}
