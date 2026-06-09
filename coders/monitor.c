/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:39:54 by asmounci          #+#    #+#             */
/*   Updated: 2026/06/09 19:05:27 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wake_all_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->params.nb_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

static int	check_burnout(t_sim *sim, int i, long now)
{
	long	last;

	pthread_mutex_lock(&sim->state_mutex);
	last = sim->coders[i].last_compile_time;
	pthread_mutex_unlock(&sim->state_mutex);
	if (now - last > sim->params.time_to_burnout)
	{
		log_state(sim, sim->coders[i].id, "burned out");
		return (1);
	}
	return (0);
}

static void	stop_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->state_mutex);
	wake_all_dongles(sim);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	long	now;
	int		i;

	sim = (t_sim *)arg;
	while (!is_stopped(sim))
	{
		i = 0;
		now = get_time_ms();
		while (i < sim->params.nb_coders)
		{
			if (check_burnout(sim, i, now))
			{
				stop_simulation(sim);
				return (NULL);
			}
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
