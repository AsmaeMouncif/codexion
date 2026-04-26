# include "codexion.h"

void log_state(t_sim *sim, int id, char *msg)
{
    long timestamp;

    pthread_mutex_lock(&sim->log_mutex);
    if (sim->stop == 1)
    {
        pthread_mutex_unlock(&sim->log_mutex);
        return;
    }
    timestamp = get_time_ms() - sim->start_time;
    printf("%ld %d %s\n", timestamp, id, msg);
    pthread_mutex_unlock(&sim->log_mutex);
}
