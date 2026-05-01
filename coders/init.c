#include "codexion.h"

t_coder	*init_coders(int n)
{
	t_coder	*coders;
	int		i;

	coders = malloc(sizeof(t_coder) * n);
	if (coders == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		coders[i].id = i + 1;
		coders[i].compile_count = 0;
		i++;
	}
	return (coders);
}

t_dongle	*init_dongles(int n)
{
	t_dongle	*dongles;
	int			i;

	dongles = malloc(sizeof(t_dongle) * n);
	if (dongles == NULL)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dongles[i].available = 1;
		pthread_mutex_init(&dongles[i].mutex, NULL);
		i++;
	}
	return (dongles);
}

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
