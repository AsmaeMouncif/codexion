#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_coder
{
	int	id;
	int	compile_count;
}	t_coder;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				available;
}	t_dongle;

int			check_args(int ac, char **av);
int			is_valid_number(char *str, int allow_zero);
int			is_valid_scheduler(char *str);
t_coder		*init_coders(int n);
t_dongle	*init_dongles(int n);
void		cleanup(t_coder *coders, t_dongle *dongles, int n);

#endif
