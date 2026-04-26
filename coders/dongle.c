
# include "codexion.h"

void dongle_acquire(t_sim *sim, int coder_id, int dongle_id)
{
    t_dongle *d = &sim->dongles[dongle_id];
    t_request req;

    req.coder_id = coder_id;
    req.arrival_time = get_time_ms();
    req.deadline = sim->coders[coder_id - 1].last_compile_time
                    + sim->params.time_to_burnout;

    pthread_mutex_lock(&d->mutex);

    heap_push(d, req, sim->params.scheduler);

    while (sim->stop == 0 &&
          (d->in_use == 1 ||
           (d->last_release_time > 0 &&
            get_time_ms() - d->last_release_time < sim->params.dongle_cooldown) ||
           heap_peek(d).coder_id != coder_id))
    {
        pthread_cond_wait(&d->cond, &d->mutex);
    }

    if (sim->stop == 1)
    {
        pthread_mutex_unlock(&d->mutex);
        return;
    }

    heap_pop(d, sim->params.scheduler);
    d->in_use = 1;

    pthread_mutex_unlock(&d->mutex);

    log_state(sim, coder_id, "has taken a dongle");
}

void dongle_release(t_sim *sim, int dongle_id)
{
    t_dongle *d = &sim->dongles[dongle_id];

    pthread_mutex_lock(&d->mutex);

    d->in_use = 0;
    d->last_release_time = get_time_ms();

    pthread_cond_broadcast(&d->cond);

    pthread_mutex_unlock(&d->mutex);
}