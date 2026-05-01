#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_coder
{
	int	id;
	int	compile_count;
}		t_coder;

int		is_valid_number(char *str, int allow_zero);
int		is_valid_scheduler(char *str);
t_coder	*init_simulation(char **av);

#endif
