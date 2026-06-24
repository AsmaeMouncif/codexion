/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:39:22 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/19 10:39:24 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	update_compile_time(t_coder *coder, t_sim *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	coder->last_compile_time = get_time_ms();
	pthread_mutex_unlock(&sim->state_mutex);
}

static int	do_compile(t_coder *coder, t_sim *sim)
{
	take_dongles(coder, sim);
	if (is_stopped(sim) != 0)
	{
		release_dongles(coder, sim);
		return (1);
	}
	update_compile_time(coder, sim);
	log_state(sim, coder->id, "is compiling");
	usleep(sim->params.time_to_compile * 1000);
	if (is_stopped(sim) != 0)
	{
		release_dongles(coder, sim);
		return (1);
	}
	release_dongles(coder, sim);
	pthread_mutex_lock(&sim->state_mutex);
	coder->compile_count++;
	pthread_mutex_unlock(&sim->state_mutex);
	return (0);
}

static int	do_debug_refactor(t_coder *coder, t_sim *sim)
{
	log_state(sim, coder->id, "is debugging");
	usleep(sim->params.time_to_debug * 1000);
	if (is_stopped(sim) != 0)
		return (1);
	log_state(sim, coder->id, "is refactoring");
	usleep(sim->params.time_to_refactor * 1000);
	return (is_stopped(sim));
}

static int	coder_cycle(t_coder *coder, t_sim *sim)
{
	if (do_compile(coder, sim) != 0)
		return (1);
	if (all_coders_done(sim) != 0)
	{
		pthread_mutex_lock(&sim->state_mutex);
		sim->stop = 1;
		pthread_mutex_unlock(&sim->state_mutex);
		return (1);
	}
	return (do_debug_refactor(coder, sim));
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	if (coder->id % 2 == 0)
		usleep(1000);
	while (is_stopped(sim) == 0)
	{
		if (all_coders_done(sim) != 0)
		{
			pthread_mutex_lock(&sim->state_mutex);
			sim->stop = 1;
			pthread_mutex_unlock(&sim->state_mutex);
			return (NULL);
		}
		if (coder_cycle(coder, sim) != 0)
			return (NULL);
	}
	return (NULL);
}
