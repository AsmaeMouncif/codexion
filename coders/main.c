#include "codexion.h"

int	main(int ac, char **av)
{
	int	i;

	if (ac != 9)
		return (write(2, "Error: invalid number of arguments\n", 35), 1);
	i = 1;
	while (i <= 7)
	{
		if (!is_valid_number(av[i]))
			return (write(2, "Error: invalid numeric argument\n", 32), 1);
		i++;
	}
	if (!is_valid_scheduler(av[8]))
		return (write(2, "Error: invalid scheduler\n", 25), 1);
	return (0);
}
