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
				return (NULL);
			}
			i++;
		}
		usleep(500);
	}
	return (NULL);
}
