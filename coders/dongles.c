/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 12:08:56 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/09 12:08:57 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

void	take_dongles(t_coder *coder, t_sim *sim)
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

void	release_dongles(t_coder *coder, t_sim *sim)
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
