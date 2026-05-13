/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 12:09:13 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/09 12:09:15 by asmounci         ###   ########.fr       */
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

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	long	now;
	int		i;

	sim = (t_sim *)arg;
	while (!sim->stop)
	{
		i = 0;
		now = get_time_ms();
		while (i < sim->params.nb_coders)
		{
			if (now - sim->coders[i].last_compile_time
				> sim->params.time_to_burnout)
			{
				log_state(sim, sim->coders[i].id, "burned out");
				sim->stop = 1;
				wake_all_dongles(sim);
				return (NULL);
			}
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
