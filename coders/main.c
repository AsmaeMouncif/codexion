/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 12:48:47 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/04 12:48:48 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_coder		*coders;
	t_dongle	*dongles;
	int			n;
	long		start_time;

	if (check_args(ac, av))
		return (1);
	n = atoi(av[1]);
	start_time = get_time_ms();
	coders = init_coders(n);
	if (coders == NULL)
		return (write(2, "Error: malloc failed\n", 21), 1);
	dongles = init_dongles(n);
	if (dongles == NULL)
		return (free(coders), write(2, "Error: malloc failed\n", 21), 1);
	cleanup(coders, dongles, n);
	return (0);
}
