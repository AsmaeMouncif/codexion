#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>

typedef struct s_coder
{
	int			id;
	int			compile_count;
	pthread_t	thread;
	long		last_compile_time;
	long		deadline;
}	t_coder;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				available;
	long			released_at;
}	t_dongle;

typedef struct s_params
{
	int	nb_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	nb_compiles_required;
	int	dongle_cooldown;
	int	scheduler;
}	t_params;

int			check_args(int ac, char **av);
int			is_valid_number(char *str, int allow_zero);
int			is_valid_scheduler(char *str);
t_params	parse_args(char **av);
t_coder		*init_coders(int n);
t_dongle	*init_dongles(int n);
void		cleanup(t_coder *coders, t_dongle *dongles, int n);

#endif
