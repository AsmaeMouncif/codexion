/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 16:15:02 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/09 16:15:17 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap_waiters(t_waiter *a, t_waiter *b)
{
	t_waiter	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	sift_up(t_dongle *d, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (d->heap[index].key >= d->heap[parent].key)
			break ;
		swap_waiters(&d->heap[index], &d->heap[parent]);
		index = parent;
	}
}

static void	sift_down(t_dongle *d, int index)
{
	int	left;
	int	right;
	int	min;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		min = index;
		if (left < d->nb_waiters && d->heap[left].key < d->heap[min].key)
			min = left;
		if (right < d->nb_waiters && d->heap[right].key < d->heap[min].key)
			min = right;
		if (min == index)
			break ;
		swap_waiters(&d->heap[index], &d->heap[min]);
		index = min;
	}
}

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
	sift_up(d, i);
	sift_down(d, i);
}
