/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/20 13:08:38 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/20 13:08:40 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_waiters(t_waiter *a, t_waiter *b)
{
	t_waiter	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	sift_up(t_dongle *d, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (d->heap[index].key > d->heap[parent].key)
			break ;
		if (d->heap[index].key == d->heap[parent].key
			&& d->heap[index].coder_id >= d->heap[parent].coder_id)
			break ;
		swap_waiters(&d->heap[index], &d->heap[parent]);
		index = parent;
	}
}

void	sift_down(t_dongle *d, int index)
{
	int	left;
	int	right;
	int	min;

	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		min = index;
		if (left < d->nb_waiters
			&& (d->heap[left].key < d->heap[min].key
				|| (d->heap[left].key == d->heap[min].key
					&& d->heap[left].coder_id < d->heap[min].coder_id)))
			min = left;
		if (right < d->nb_waiters
			&& (d->heap[right].key < d->heap[min].key
				|| (d->heap[right].key == d->heap[min].key
					&& d->heap[right].coder_id < d->heap[min].coder_id)))
			min = right;
		if (min == index)
			break ;
		swap_waiters(&d->heap[index], &d->heap[min]);
		index = min;
	}
}
