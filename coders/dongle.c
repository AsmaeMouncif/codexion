
# include "codexion.h"
# include <time.h>

void dongle_acquire(t_sim *sim, int coder_id, int dongle_id)
{
    t_dongle *d = &sim->dongles[dongle_id];
    t_request req;
    struct timespec ts;
    long wake_ms;
    long release_time;
    int stopped;

    req.coder_id = coder_id;
    req.arrival_time = get_time_ms();
    req.deadline = sim->coders[coder_id - 1].last_compile_time
                    + sim->params.time_to_burnout;

    pthread_mutex_lock(&d->mutex);

    heap_push(d, req, sim->params.scheduler);

    /* Check stop flag once before loop, avoid nested locks */
    pthread_mutex_lock(&sim->stop_mutex);
    stopped = sim->stop;
    pthread_mutex_unlock(&sim->stop_mutex);

    while (stopped == 0 &&
          (d->in_use == 1 ||
           (d->last_release_time > 0 &&
            get_time_ms() - d->last_release_time < sim->params.dongle_cooldown) ||
           heap_peek(d).coder_id != coder_id))
    {
        release_time = d->last_release_time;
        if (release_time > 0)
            wake_ms = release_time + sim->params.dongle_cooldown + 1;
        else
            wake_ms = get_time_ms() + 1;
        ts.tv_sec = wake_ms / 1000;
        ts.tv_nsec = (wake_ms % 1000) * 1000000L;
        pthread_cond_timedwait(&d->cond, &d->mutex, &ts);

        /* Check stop flag again after timedwait */
        pthread_mutex_lock(&sim->stop_mutex);
        stopped = sim->stop;
        pthread_mutex_unlock(&sim->stop_mutex);
    }

    if (stopped)
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