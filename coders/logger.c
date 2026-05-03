#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	log_state(long start_time, int id, char *msg)
{
	long	timestamp;

	timestamp = get_time_ms() - start_time;
	printf("%ld %d %s\n", timestamp, id, msg);
}
