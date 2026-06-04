/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:39:45 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/19 10:39:47 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	log_state(t_sim *sim, int id, char *msg)
{
	long	timestamp;

	// pthread_mutex_lock(&sim->state_mutex);
	// if (sim->stop)
	// {
	// 	pthread_mutex_unlock(&sim->state_mutex);
	// 	return ;
	// }
	pthread_mutex_lock(&sim->log_mutex);
	timestamp = get_time_ms() - sim->start_time;
	printf("%ld %d %s\n", timestamp, id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
	// pthread_mutex_unlock(&sim->state_mutex);
}
