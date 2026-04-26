#include "codexion.h"

static void stop_sim(t_sim *sim)
{
    int i;

    sim->stop = 1;
    i = 0;
    while (i < sim->params.number_of_coders)
    {
        pthread_cond_broadcast(&sim->dongles[i].cond);
        i++;
    }
}

static int check_burnout(t_sim *sim)
{
    int i;
    long now;

    i = 0;
    while (i < sim->params.number_of_coders)
    {
        now = get_time_ms();

        if (now - sim->coders[i].last_compile_time
            > sim->params.time_to_burnout)
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

    if (sim->params.number_of_compiles_required < 0)
        return (0);

    i = 0;
    while (i < sim->params.number_of_coders)
    {
        if (sim->coders[i].nb_compiles
            < sim->params.number_of_compiles_required)
            return (0);
        i++;
    }

    stop_sim(sim);
    return (1);
}

void *monitor_routine(void *arg)
{
    t_sim *sim;

    sim = (t_sim *)arg;

    while (sim->stop == 0)
    {
        if (check_burnout(sim))
            return (NULL);

        if (check_compiles(sim))
            return (NULL);

        sleep_ms(1);
    }
    return (NULL);
}