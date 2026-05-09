#include "codexion.h"

int	is_stopped(t_sim *sim)
{
	return (sim->stop);
}

int	all_coders_done(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->params.nb_coders)
	{
		if (sim->coders[i].compile_count < sim->params.nb_compiles_required)
			return (0);
		i++;
	}
	return (1);
}
