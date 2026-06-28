/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:39:54 by asmounci          #+#    #+#             */
/*   Updated: 2026/06/28 22:20:07 by asmounci         ###   ########.fr       */
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

static void	stop_simulation(t_sim *sim)
{
	pthread_mutex_lock(&sim->state_mutex);
	sim->stop = 1;
	pthread_mutex_unlock(&sim->state_mutex);
	wake_all_dongles(sim);
}

static int	check_burnout(t_sim *sim, int i, long now)
{
	long	last;
	long	timestamp;

	pthread_mutex_lock(&sim->state_mutex);
	last = sim->coders[i].last_compile_time;
	pthread_mutex_unlock(&sim->state_mutex);
	if (now - last > sim->params.time_to_burnout)
	{
		stop_simulation(sim);
		timestamp = get_time_ms() - sim->start_time;
		usleep(1000);
		printf("%ld %d %s\n", timestamp, sim->coders[i].id, "burned out");
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	long	now;
	int		i;

	sim = (t_sim *)arg;
	while (is_stopped(sim) == 0)
	{
		i = 0;
		now = get_time_ms();
		while (i < sim->params.nb_coders)
		{
			if (check_burnout(sim, i, now) != 0)
				return (NULL);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
