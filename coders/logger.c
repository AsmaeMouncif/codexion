# include "codexion.h"

void log_state(t_sim *sim, int id, char *msg)
{
    long timestamp;
    int stopped;

    pthread_mutex_lock(&sim->stop_mutex);
    stopped = sim->stop;
    pthread_mutex_unlock(&sim->stop_mutex);

    if (stopped)
        return;

    pthread_mutex_lock(&sim->log_mutex);
    timestamp = get_time_ms() - sim->start_time;
    printf("%ld %d %s\n", timestamp, id, msg);
    pthread_mutex_unlock(&sim->log_mutex);
}
