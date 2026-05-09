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

static void	take_one_dongle(t_coder *coder, t_sim *sim, int idx)
{
	t_dongle	*d;

	d = &sim->dongles[idx];
	while (!is_stopped(sim))
	{
		pthread_mutex_lock(&d->mutex);
		if (d->available
			&& (get_time_ms() - d->released_at) >= sim->params.dongle_cooldown)
		{
			d->available = 0;
			pthread_mutex_unlock(&d->mutex);
			log_state(sim, coder->id, "has taken a dongle");
			return ;
		}
		pthread_mutex_unlock(&d->mutex);
		usleep(500);
	}
}

static void	take_dongles(t_coder *coder, t_sim *sim)
{
	int	left;
	int	right;

	left = coder->id - 1;
	right = coder->id % sim->params.nb_coders;
	if (coder->id == sim->params.nb_coders)
	{
		take_one_dongle(coder, sim, right);
		take_one_dongle(coder, sim, left);
	}
	else
	{
		take_one_dongle(coder, sim, left);
		take_one_dongle(coder, sim, right);
	}
}

static void	release_dongles(t_coder *coder, t_sim *sim)
{
	int	left;
	int	right;

	left = coder->id - 1;
	right = coder->id % sim->params.nb_coders;
	pthread_mutex_lock(&sim->dongles[left].mutex);
	sim->dongles[left].available = 1;
	sim->dongles[left].released_at = get_time_ms();
	pthread_mutex_unlock(&sim->dongles[left].mutex);
	pthread_mutex_lock(&sim->dongles[right].mutex);
	sim->dongles[right].available = 1;
	sim->dongles[right].released_at = get_time_ms();
	pthread_mutex_unlock(&sim->dongles[right].mutex);
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
		take_dongles(coder, sim);
		if (is_stopped(sim))
			return (NULL);
		log_state(sim, coder->id, "is compiling");
		usleep(sim->params.time_to_compile * 1000);
		if (is_stopped(sim))
		{
			release_dongles(coder, sim);
			return (NULL);
		}
		release_dongles(coder, sim);
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
