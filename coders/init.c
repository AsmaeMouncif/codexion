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
		dongles[i].available = 1;
		dongles[i].released_at = 0;
		dongles[i].nb_waiters = 0;
		dongles[i].capacity = n;
		dongles[i].heap = malloc(sizeof(t_waiter) * n);
		if (dongles[i].heap == NULL
			|| pthread_mutex_init(&dongles[i].mutex, NULL) != 0
			|| pthread_cond_init(&dongles[i].cond, NULL) != 0)
		{
			free_dongles(dongles, i);
			return (NULL);
		}
		i++;
	}
	return (dongles);
}
