#include "codexion.h"

static void release_both(t_sim *sim, int left, int right)
{
    dongle_release(sim, left);
    dongle_release(sim, right);
}

void *coder_routine(void *arg)
{
    t_coder    *c;
    t_sim      *sim;
    int        left;
    int        right;
    int        stopped;

    c = (t_coder *)arg;
    sim = c->sim;

    if (sim->params.number_of_coders == 1)
    {
        sleep_ms(sim->params.time_to_burnout);
        return (NULL);
    }

    left = c->id - 1;
    right = c->id % sim->params.number_of_coders;

    if (c->id == sim->params.number_of_coders)
    {
        left = 0;
        right = c->id - 1;
    }

    pthread_mutex_lock(&sim->stop_mutex);
    stopped = sim->stop;
    pthread_mutex_unlock(&sim->stop_mutex);

    while (stopped == 0)
    {
        dongle_acquire(sim, c->id, left);

        pthread_mutex_lock(&sim->stop_mutex);
        stopped = sim->stop;
        pthread_mutex_unlock(&sim->stop_mutex);
        if (stopped)
            return (dongle_release(sim, left), NULL);

        dongle_acquire(sim, c->id, right);

        pthread_mutex_lock(&sim->stop_mutex);
        stopped = sim->stop;
        pthread_mutex_unlock(&sim->stop_mutex);
        if (stopped)
            return (release_both(sim, left, right), NULL);

        log_state(sim, c->id, "is compiling");

        coder_set_last_compile_time(c, get_time_ms());
        coder_inc_nb_compiles(c);

        sleep_ms(sim->params.time_to_compile);

        release_both(sim, left, right);

        log_state(sim, c->id, "is debugging");
        sleep_ms(sim->params.time_to_debug);

        log_state(sim, c->id, "is refactoring");
        sleep_ms(sim->params.time_to_refactor);

        pthread_mutex_lock(&sim->stop_mutex);
        stopped = sim->stop;
        pthread_mutex_unlock(&sim->stop_mutex);
    }

    return (NULL);
}
