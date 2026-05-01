#include "codexion.h"

void	cleanup(t_coder *coders, t_dongle *dongles, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&dongles[i].mutex);
		i++;
	}
	free(dongles);
	free(coders);
}
