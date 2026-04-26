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

    while (sim->stop == 0)
    {
        dongle_acquire(sim, c->id, left);
        if (sim->stop)
            return (dongle_release(sim, left), NULL);

        dongle_acquire(sim, c->id, right);
        if (sim->stop)
            return (release_both(sim, left, right), NULL);

        log_state(sim, c->id, "is compiling");

        c->last_compile_time = get_time_ms();
        c->nb_compiles++;

        sleep_ms(sim->params.time_to_compile);

        release_both(sim, left, right);

        log_state(sim, c->id, "is debugging");
        sleep_ms(sim->params.time_to_debug);

        log_state(sim, c->id, "is refactoring");
        sleep_ms(sim->params.time_to_refactor);
    }

    return (NULL);
}
