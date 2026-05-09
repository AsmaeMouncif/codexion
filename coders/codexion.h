/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/04 12:48:34 by asmounci          #+#    #+#             */
/*   Updated: 2026/05/04 18:12:09 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct s_sim	t_sim;

typedef struct s_coder
{
	int			id;
	int			compile_count;
	pthread_t	thread;
	long		last_compile_time;
	long		deadline;
	t_sim		*sim;
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

typedef struct s_sim
{
	t_params		params;
	t_coder			*coders;
	t_dongle		*dongles;
	long			start_time;
	int				stop;
	pthread_mutex_t	log_mutex;
	pthread_t		monitor;
}	t_sim;

int			check_args(int ac, char **av);
int			is_valid_number(char *str, int allow_zero);
int			is_valid_scheduler(char *str);
t_params	parse_args(char **av);
t_coder		*init_coders(int n);
t_dongle	*init_dongles(int n);
void		cleanup(t_coder *coders, t_dongle *dongles, int n);
long		get_time_ms(void);
void		log_state(t_sim *sim, int id, char *msg);
int			is_stopped(t_sim *sim);
int			all_coders_done(t_sim *sim);
void		take_dongles(t_coder *coder, t_sim *sim);
void		release_dongles(t_coder *coder, t_sim *sim);
void		*coder_routine(void *arg);
int			start_simulation(t_sim *sim);
void		*monitor_routine(void *arg);

#endif
