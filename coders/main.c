/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 15:11:13 by asmounci          #+#    #+#             */
/*   Updated: 2026/04/26 09:16:57 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int  launch_threads(t_sim *sim)
{
    int i;

    i = 0;
    while (i < sim->params.number_of_coders)
    {
        if (pthread_create(&sim->coders[i].thread, NULL,
                coder_routine, &sim->coders[i]) != 0)
        {
            sim_set_stop(sim, 1);
            while (i < sim->params.number_of_coders)
                pthread_cond_broadcast(&sim->dongles[i++].cond);
            return (1);
        }
        i++;
    }
    if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
    {
        sim_set_stop(sim, 1);
        i = 0;
        while (i < sim->params.number_of_coders)
            pthread_cond_broadcast(&sim->dongles[i++].cond);
        return (1);
    }
    return (0);
}

static void join_threads(t_sim *sim)
{
    int i;

    pthread_join(sim->monitor, NULL);
    i = 0;
    while (i < sim->params.number_of_coders)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
}

int main(int ac, char **av)
{
    t_sim   sim;
    int     i;

    if (ac != 9)
        return (write(2, "Error: invalid number of arguments\n", 35), 1);
    i = 1;
    while (i <= 7)
    {
        if (!is_valid_number(av[i]))
            return (write(2, "Error: invalid numeric argument\n", 32), 1);
        i++;
    }
    if (!is_valid_scheduler(av[8]))
        return (write(2, "Error: invalid scheduler\n", 25), 1);
    if (init_sim(&sim, av) != 0)
        return (write(2, "Error: init failed\n", 19), 1);
    if (launch_threads(&sim) != 0)
    {
        join_threads(&sim);
        cleanup_sim(&sim);
        return (1);
    }
    join_threads(&sim);
    cleanup_sim(&sim);
    return (0);
}
