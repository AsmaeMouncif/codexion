/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 12:49:15 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/04 12:49:16 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_valid_number(char *str, int allow_zero)
{
	int	i;

	i = 0;
	if (str == NULL || str[0] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (allow_zero != 0)
		return (atoi(str) >= 0);
	else
		return (atoi(str) > 0);
}

int	is_valid_scheduler(char *str)
{
	if (str == NULL || str[0] == '\0')
		return (0);
	if (strcmp(str, "fifo") == 0 || strcmp(str, "edf") == 0)
		return (1);
	return (0);
}

t_params	parse_args(char **av)
{
	t_params	p;

	p.nb_coders = atoi(av[1]);
	p.time_to_burnout = atoi(av[2]);
	p.time_to_compile = atoi(av[3]);
	p.time_to_debug = atoi(av[4]);
	p.time_to_refactor = atoi(av[5]);
	p.nb_compiles_required = atoi(av[6]);
	p.dongle_cooldown = atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		p.scheduler = 0;
	else
		p.scheduler = 1;
	return (p);
}
