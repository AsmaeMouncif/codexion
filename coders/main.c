#include "codexion.h"

int	main(int ac, char **av)
{
	t_coder		*coders;
	t_dongle	*dongles;
	int			n;

	if (check_args(ac, av))
		return (1);
	n = atoi(av[1]);
	coders = init_coders(n);
	if (coders == NULL)
		return (write(2, "Error: malloc failed\n", 21), 1);
	dongles = init_dongles(n);
	if (dongles == NULL)
		return (free(coders), write(2, "Error: malloc failed\n", 21), 1);
	cleanup(coders, dongles, n);
	return (0);
}
