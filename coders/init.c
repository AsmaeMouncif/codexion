#include "codexion.h"

t_coder	*init_simulation(char **av)
{
	int		n;
	t_coder	*coders;
	int		i;

	n = atoi(av[1]);
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
