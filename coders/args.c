#include "codexion.h"

int	check_args(int ac, char **av)
{
	if (ac != 9)
		return (write(2, "Error: invalid number of arguments\n", 35), 1);
	if (!is_valid_number(av[1], 0))
		return (write(2, "Error: invalid number_of_coders\n", 32), 1);
	if (!is_valid_number(av[2], 0))
		return (write(2, "Error: invalid time_to_burnout\n", 31), 1);
	if (!is_valid_number(av[3], 0))
		return (write(2, "Error: invalid time_to_compile\n", 31), 1);
	if (!is_valid_number(av[4], 0))
		return (write(2, "Error: invalid time_to_debug\n", 29), 1);
	if (!is_valid_number(av[5], 0))
		return (write(2, "Error: invalid time_to_refactor\n", 32), 1);
	if (!is_valid_number(av[6], 0))
		return (write(2, "Error: invalid number_of_compiles_required\n", 43),
			1);
	if (!is_valid_number(av[7], 1))
		return (write(2, "Error: invalid dongle_cooldown\n", 31), 1);
	if (!is_valid_scheduler(av[8]))
		return (write(2, "Error: invalid scheduler\n", 25), 1);
	return (0);
}
