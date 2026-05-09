/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 12:08:45 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/09 12:08:47 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	coder_cycle(t_coder *coder, t_sim *sim)
{
	take_dongles(coder, sim);
	if (is_stopped(sim))
		return (1);
	coder->last_compile_time = get_time_ms();
	log_state(sim, coder->id, "is compiling");
	usleep(sim->params.time_to_compile * 1000);
	if (is_stopped(sim))
	{
		release_dongles(coder, sim);
		return (1);
	}
	release_dongles(coder, sim);
	coder->compile_count++;
	if (all_coders_done(sim))
	{
		sim->stop = 1;
		return (1);
	}
	log_state(sim, coder->id, "is debugging");
	usleep(sim->params.time_to_debug * 1000);
	if (is_stopped(sim))
		return (1);
	log_state(sim, coder->id, "is refactoring");
	usleep(sim->params.time_to_refactor * 1000);
	return (is_stopped(sim));
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (!sim->stop)
	{
		if (all_coders_done(sim))
		{
			sim->stop = 1;
			return (NULL);
		}
		if (coder_cycle(coder, sim))
			return (NULL);
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
		sim->coders[i].last_compile_time = sim->start_time;
		pthread_create(&sim->coders[i].thread, NULL, coder_routine,
			&sim->coders[i]);
		i++;
	}
	pthread_create(&sim->monitor, NULL, monitor_routine, sim);
	i = 0;
	while (i < n)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
	return (0);
}
