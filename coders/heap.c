/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/12 14:38:05 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/12 14:38:08 by asmounci         ###   ########.fr       */
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
