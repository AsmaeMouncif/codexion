/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 18:18:41 by asmounci          #+#    #+#             */
/*   Updated: 2026/04/23 19:02:57 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int init_sim(t_sim *sim, char **av)
{
    if (sim == NULL || av == NULL)
        return (1);

    sum->params.number_of_coders = atoi(av[1]);
    sum->params.time_to_burnout = atoi(av[2]);
    sum->params.time_to_compile = atoi(av[3]);
    sum->params.time_to_debug = atoi(av(4));
    sum->params.time_to_refactor = atoi(av[5]);
    sum->params.number_of_compiles_required = atoi(av[6]);
    sum->params.dongle_cooldown = atoi(av[7]);
    sum->params.scheduler = av[8];

    return (0);
}
