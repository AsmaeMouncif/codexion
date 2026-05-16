/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 12:48:39 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/04 13:03:49 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_coder	*init_coders(int n)
{
	t_coder	*coders;
	int		i;

	coders = malloc(sizeof(t_coder) * n);
	if (coders == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		coders[i].id = i + 1;
		coders[i].compile_count = 0;
		coders[i].last_compile_time = 0;
		coders[i].deadline = 0;
		i++;
	}
	return (coders);
}

static void	free_dongles(t_dongle *d, int i)
{
	while (i > 0)
	{
		i--;
		free(d[i].heap);
		pthread_mutex_destroy(&d[i].mutex);
		pthread_cond_destroy(&d[i].cond);
	}
	free(d);
}

static int	init_one_dongle(t_dongle *d, int n, t_dongle *all, int i)
{
	d->available = 1;
	d->released_at = 0;
	d->nb_waiters = 0;
	d->capacity = n;
	d->heap = malloc(sizeof(t_waiter) * n);
	if (d->heap == NULL)
		return (free_dongles(all, i), 0);
	if (pthread_mutex_init(&d->mutex, NULL) != 0)
	{
		free(d->heap);
		return (free_dongles(all, i), 0);
	}
	if (pthread_cond_init(&d->cond, NULL) != 0)
	{
		free(d->heap);
		pthread_mutex_destroy(&d->mutex);
		return (free_dongles(all, i), 0);
	}
	return (1);
}

t_dongle	*init_dongles(int n)
{
	t_dongle	*dongles;
	int			i;

	dongles = malloc(sizeof(t_dongle) * n);
	if (dongles == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		if (init_one_dongle(&dongles[i], n, dongles, i) == 0)
			return (NULL);
		i++;
	}
	return (dongles);
}
