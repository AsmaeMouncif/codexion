/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 12:48:28 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/04 12:48:29 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	cleanup(t_coder *coders, t_dongle *dongles, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		pthread_cond_destroy(&dongles[i].cond);
		free(dongles[i].heap);
		i++;
	}
	free(dongles);
	free(coders);
}
