#include "codexion.h"

static void stop_sim(t_sim *sim)
{
    int i;

    sim_set_stop(sim, 1);
    i = 0;
    while (i < sim->params.number_of_coders)
    {
        pthread_mutex_lock(&sim->dongles[i].mutex);
        pthread_cond_broadcast(&sim->dongles[i].cond);
        pthread_mutex_unlock(&sim->dongles[i].mutex);
        i++;
    }
}

static int check_burnout(t_sim *sim)
{
    int i;
    long now;
    long last_compile;

    i = 0;
    while (i < sim->params.number_of_coders)
    {
        now = get_time_ms();
        last_compile = coder_get_last_compile_time(&sim->coders[i]);

        if (now - last_compile > sim->params.time_to_burnout)
        {
            log_state(sim, sim->coders[i].id, "burned out");
            stop_sim(sim);
            return (1);
        }
        i++;
    }
    return (0);
}

static int check_compiles(t_sim *sim)
{
    int i;
    int nb_compiles;

    if (sim->params.number_of_compiles_required < 0)
        return (0);

    i = 0;
    while (i < sim->params.number_of_coders)
    {
        nb_compiles = coder_get_nb_compiles(&sim->coders[i]);
        if (nb_compiles < sim->params.number_of_compiles_required)
            return (0);
        i++;
    }

    stop_sim(sim);
    return (1);
}

void *monitor_routine(void *arg)
{
    t_sim *sim;
    int stopped;

    sim = (t_sim *)arg;

    pthread_mutex_lock(&sim->stop_mutex);
    stopped = sim->stop;
    pthread_mutex_unlock(&sim->stop_mutex);

    while (stopped == 0)
    {
        if (check_burnout(sim))
            return (NULL);

        if (check_compiles(sim))
            return (NULL);

        sleep_ms(1);

        pthread_mutex_lock(&sim->stop_mutex);
        stopped = sim->stop;
        pthread_mutex_unlock(&sim->stop_mutex);
    }
    return (NULL);
}