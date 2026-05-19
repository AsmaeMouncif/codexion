/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 10:39:34 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/19 10:39:36 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongles(t_coder *coder, t_sim *sim)
{
	int	left;
	int	right;

	left = coder->id - 1;
	right = coder->id % sim->params.nb_coders;
	if (coder->id == sim->params.nb_coders)
	{
		take_one_dongle(coder, sim, right);
		take_one_dongle(coder, sim, left);
	}
	else
	{
		take_one_dongle(coder, sim, left);
		take_one_dongle(coder, sim, right);
	}
}

void	release_dongles(t_coder *coder, t_sim *sim)
{
	int	left;
	int	right;

	left = coder->id - 1;
	right = coder->id % sim->params.nb_coders;
	release_one(sim, left);
	release_one(sim, right);
}
