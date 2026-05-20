/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 12:58:41 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/20 12:58:43 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_push(t_dongle *d, t_waiter w)
{
	int	i;

	if (d->nb_waiters >= d->capacity)
		return ;
	i = d->nb_waiters;
	d->heap[i] = w;
	d->nb_waiters++;
	sift_up(d, i);
}

void	heap_pop(t_dongle *d)
{
	if (d->nb_waiters <= 0)
		return ;
	d->heap[0] = d->heap[d->nb_waiters - 1];
	d->nb_waiters--;
	if (d->nb_waiters > 0)
		sift_down(d, 0);
}

int	heap_top_id(t_dongle *d)
{
	if (d->nb_waiters <= 0)
		return (-1);
	return (d->heap[0].coder_id);
}

void	heap_remove(t_dongle *d, int coder_id)
{
	int	i;

	i = 0;
	while (i < d->nb_waiters)
	{
		if (d->heap[i].coder_id == coder_id)
			break ;
		i++;
	}
	if (i == d->nb_waiters)
		return ;
	d->heap[i] = d->heap[d->nb_waiters - 1];
	d->nb_waiters--;
	if (i >= d->nb_waiters)
		return ;
	if (i > 0 && d->heap[i].key < d->heap[(i - 1) / 2].key)
		sift_up(d, i);
	else
		sift_down(d, i);
}
