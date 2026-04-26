
# include "codexion.h"

long get_time_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L) + (tv.tv_usec / 1000);
}

void sleep_ms(long ms)
{
    usleep(ms * 1000);
}

/*
** Thread-safe getter/setter for sim->stop
** Uses dedicated stop_mutex for synchronization
*/
int sim_should_stop(t_sim *sim)
{
    int stop_value;

    if (sim->stop_mutex_initialized)
        pthread_mutex_lock(&sim->stop_mutex);
    stop_value = sim->stop;
    if (sim->stop_mutex_initialized)
        pthread_mutex_unlock(&sim->stop_mutex);
    return (stop_value);
}

void sim_set_stop(t_sim *sim, int value)
{
    if (sim->stop_mutex_initialized)
        pthread_mutex_lock(&sim->stop_mutex);
    sim->stop = value;
    if (sim->stop_mutex_initialized)
        pthread_mutex_unlock(&sim->stop_mutex);
}

/*
** Thread-safe getters/setters for coder fields
*/
int coder_get_nb_compiles(t_coder *coder)
{
    int value;

    pthread_mutex_lock(&coder->mutex);
    value = coder->nb_compiles;
    pthread_mutex_unlock(&coder->mutex);
    return (value);
}

void coder_inc_nb_compiles(t_coder *coder)
{
    pthread_mutex_lock(&coder->mutex);
    coder->nb_compiles++;
    pthread_mutex_unlock(&coder->mutex);
}

long coder_get_last_compile_time(t_coder *coder)
{
    long value;

    pthread_mutex_lock(&coder->mutex);
    value = coder->last_compile_time;
    pthread_mutex_unlock(&coder->mutex);
    return (value);
}

void coder_set_last_compile_time(t_coder *coder, long time)
{
    pthread_mutex_lock(&coder->mutex);
    coder->last_compile_time = time;
    pthread_mutex_unlock(&coder->mutex);
}
