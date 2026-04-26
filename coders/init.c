/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 18:18:41 by asmounci          #+#    #+#             */
/*   Updated: 2026/04/26 09:16:04 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

static int init_coders(t_sim *sim)
{
    int i;

    sim->coders = malloc(sizeof(t_coder) * sim->params.number_of_coders);
    if (sim->coders == NULL)
        return (1);
    i = 0;
    while (i < sim->params.number_of_coders)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].nb_compiles = 0;
        sim->coders[i].last_compile_time = sim->start_time;
        sim->coders[i].sim = sim;
        if (pthread_mutex_init(&sim->coders[i].mutex, NULL))
            return (1);
        i++;
    }
    return (0);
}

static int init_dongles(t_sim *sim)
{
    int i;

    sim->dongles = malloc(sizeof(t_dongle) * sim->params.number_of_coders);
    if(sim->dongles == NULL)
        return (1);

    memset(sim->dongles, 0, sizeof(t_dongle) * sim->params.number_of_coders);

    i = 0;
    while (i < sim->params.number_of_coders)
    {
        if (pthread_mutex_init(&sim->dongles[i].mutex, NULL))
            return (1);
        sim->dongles[i].initialized = 1;
        if (pthread_cond_init(&sim->dongles[i].cond, NULL))
        {
            pthread_mutex_destroy(&sim->dongles[i].mutex);
            sim->dongles[i].initialized = 0;
            return (1);
        }
        sim->dongles[i].last_release_time = 0;
        sim->dongles[i].in_use = 0;
        sim->dongles[i].heap = malloc(sizeof(t_request) 
            * sim->params.number_of_coders);
        if (sim->dongles[i].heap == NULL)
            return (1);
        sim->dongles[i].heap_size = 0;
        sim->dongles[i].heap_capacity = sim->params.number_of_coders;
        sim->dongles[i].initialized = 1;
        i++;
    }
    return (0);
}

void cleanup_sim(t_sim *sim)
{
    int i;

    if (sim == NULL)
        return;
    if (sim->coders != NULL)
    {
        i = 0;
        while (i < sim->params.number_of_coders)
        {
            pthread_mutex_destroy(&sim->coders[i].mutex);
            i++;
        }
        free(sim->coders);
    }
    if (sim->dongles != NULL)
    {
        i = 0;
        while (i < sim->params.number_of_coders)
        {
            if (sim->dongles[i].initialized)
            {
                pthread_mutex_destroy(&sim->dongles[i].mutex);
                pthread_cond_destroy(&sim->dongles[i].cond);
                free(sim->dongles[i].heap);
            }
            i++;
        }
        free(sim->dongles);
    }
    if (sim->log_mutex_initialized)
        pthread_mutex_destroy(&sim->log_mutex);
    if (sim->stop_mutex_initialized)
        pthread_mutex_destroy(&sim->stop_mutex);
}

int init_sim(t_sim *sim, char **av)
{
    if (sim == NULL || av == NULL)
        return (1);
    sim->params.number_of_coders = atoi(av[1]);
    sim->params.time_to_burnout = atoi(av[2]);
    sim->params.time_to_compile = atoi(av[3]);
    sim->params.time_to_debug = atoi(av[4]);
    sim->params.time_to_refactor = atoi(av[5]);
    sim->params.number_of_compiles_required = atoi(av[6]);
    sim->params.dongle_cooldown = atoi(av[7]);
    sim->params.scheduler = av[8];
    sim->start_time = get_time_ms();
    sim->stop = 0;
    sim->log_mutex_initialized = 0;
    sim->stop_mutex_initialized = 0;if (init_coders(sim) != 0)
        return (1);
    if (init_dongles(sim) != 0)
        return (cleanup_sim(sim), 1);
    if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
        return (cleanup_sim(sim), 1);
    sim->log_mutex_initialized = 1;
    if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
        return (cleanup_sim(sim), 1);
    sim->stop_mutex_initialized = 1;
    return (0);
}
