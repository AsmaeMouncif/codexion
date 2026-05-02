#include "codexion.h"

static void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	printf("Coder %d started\n", coder->id);
	return (NULL);
}

void	start_threads(t_coder *coders, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		pthread_create(&coders[i].thread, NULL, coder_routine, &coders[i]);
		i++;
	}
	i = 0;
	while (i < n)
	{
		pthread_join(coders[i].thread, NULL);
		i++;
	}
}
