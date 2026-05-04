/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:05:29 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/04 18:18:04 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (!sim->stop)
	{
		log_state(sim, coder->id, "is compiling");
		usleep(sim->params.time_to_compile * 1000);
		log_state(sim, coder->id, "is debugging");
		usleep(sim->params.time_to_debug * 1000);
		log_state(sim, coder->id, "is refactoring");
		usleep(sim->params.time_to_refactor * 1000);
		coder->compile_count++;
	}
	return (NULL);
}

int	start_simulation(t_sim *sim)
{
	int	i;
	int	n;

	n = sim->params.nb_coders;
	i = 0;
	while (i < n)
	{
		sim->coders[i].sim = sim;
		pthread_create(&sim->coders[i].thread, NULL, coder_routine,
			&sim->coders[i]);
		i++;
	}
	i = 0;
	while (i < n)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	return (0);
}
