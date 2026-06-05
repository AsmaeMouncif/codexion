/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:39:30 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/19 10:39:31 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	get_key(t_coder *coder, t_sim *sim)
{
	if (sim->params.scheduler == 0)
		return (get_time_ms());
	return (coder->last_compile_time + sim->params.time_to_burnout);
}

static int	can_take(t_dongle *d, int coder_id, int cooldown)
{
	if (!d->available)
		return (0);
	if ((get_time_ms() - d->released_at) < cooldown)
		return (0);
	if (heap_top_id(d) != coder_id)
		return (0);
	return (1);
}

static void	wait_for_dongle(t_dongle *d, t_sim *sim)
{
	struct timespec	ts;
	long			wake_at;

	wake_at = d->released_at + sim->params.dongle_cooldown;
	ts.tv_sec = wake_at / 1000;
	ts.tv_nsec = (wake_at % 1000) * 1000000L;
	pthread_cond_timedwait(&d->cond, &d->mutex, &ts);
}

void	take_one_dongle(t_coder *coder, t_sim *sim, int idx)
{
	t_dongle	*d;
	t_waiter	w;

	d = &sim->dongles[idx];
	w.coder_id = coder->id;
	w.key = get_key(coder, sim);
	pthread_mutex_lock(&d->mutex);
	heap_push(d, w);
	while (!is_stopped(sim)
		&& !can_take(d, coder->id, sim->params.dongle_cooldown))
		wait_for_dongle(d, sim);
	if (is_stopped(sim))
	{
		heap_remove(d, coder->id);
		pthread_mutex_unlock(&d->mutex);
		return ;
	}
	heap_pop(d);
	d->available = 0;
	pthread_mutex_unlock(&d->mutex);
	log_state(sim, coder->id, "has taken a dongle");
}

void	release_one(t_sim *sim, int idx)
{
	pthread_mutex_lock(&sim->dongles[idx].mutex);
	sim->dongles[idx].available = 1;
	sim->dongles[idx].released_at = get_time_ms();
	pthread_cond_broadcast(&sim->dongles[idx].cond);
	pthread_mutex_unlock(&sim->dongles[idx].mutex);
}
