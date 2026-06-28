/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:39:00 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/19 10:39:02 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_stopped(t_sim *sim)
{
	int	val;

	pthread_mutex_lock(&sim->state_mutex);
	val = sim->stop;
	pthread_mutex_unlock(&sim->state_mutex);
	return (val);
}

int	all_coders_done(t_sim *sim)
{
	int	i;
	int	done;

	i = 0;
	done = 1;
	pthread_mutex_lock(&sim->state_mutex);
	while (i < sim->params.nb_coders)
	{
		if (sim->coders[i].compile_count < sim->params.nb_compiles_required)
		{
			done = 0;
			break ;
		}
		i++;
	}
	pthread_mutex_unlock(&sim->state_mutex);
	return (done);
}
