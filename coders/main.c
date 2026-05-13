/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 12:48:47 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/04 16:17:53 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_sim	sim;
	int		n;

	if (check_args(ac, av))
		return (1);
	sim.params = parse_args(av);
	n = sim.params.nb_coders;
	sim.start_time = get_time_ms();
	sim.stop = 0;
	sim.coders = init_coders(n);
	if (sim.coders == NULL)
		return (write(2, "Error: malloc failed\n", 21), 1);
	sim.dongles = init_dongles(n);
	if (sim.dongles == NULL)
		return (free(sim.coders), write(2, "Error: malloc failed\n", 21), 1);
	pthread_mutex_init(&sim.log_mutex, NULL);
	pthread_mutex_init(&sim.state_mutex, NULL);
	start_simulation(&sim);
	pthread_mutex_destroy(&sim.log_mutex);
	pthread_mutex_destroy(&sim.state_mutex);
	cleanup(sim.coders, sim.dongles, n);
	return (0);
}
