/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 15:05:29 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/04 21:13:38 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_stopped(t_sim *sim)
{
	return (sim->stop);
}

static int	all_coders_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->params.nb_coders)
	{
		if (sim->coders[i].compile_count < sim->params.nb_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	while (!sim->stop)
	{
		coder->last_compile_time = get_time_ms();
		if (is_stopped(sim))
			return (NULL);
		log_state(sim, coder->id, "is compiling");
		usleep(sim->params.time_to_compile * 1000);
		if (is_stopped(sim))
			return (NULL);
		coder->compile_count++;
		log_state(sim, coder->id, "is debugging");
		usleep(sim->params.time_to_debug * 1000);
		if (is_stopped(sim))
			return (NULL);
		log_state(sim, coder->id, "is refactoring");
		usleep(sim->params.time_to_refactor * 1000);
		if (is_stopped(sim))
			return (NULL);
		if (all_coders_done(sim))
			sim->stop = 1;
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
