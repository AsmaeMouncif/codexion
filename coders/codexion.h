/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 15:08:00 by asmounci          #+#    #+#             */
/*   Updated: 2026/04/26 09:15:36 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdio.h>

typedef struct s_sim t_sim;

typedef struct s_params {
    int number_of_coders;
    int time_to_burnout;
    int time_to_compile;
    int time_to_debug;
    int time_to_refactor;
    int number_of_compiles_required;
    int dongle_cooldown;
    char *scheduler;
} t_params;

typedef struct s_request
{
    int coder_id;
    long arrival_time;
    long deadline;
} t_request;

typedef struct s_dongle {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    long last_release_time;
    int in_use;
    t_request *heap;
    int heap_size;
    int heap_capacity;
    int initialized;
} t_dongle;

typedef struct s_coder {
    int id;
    int nb_compiles;
    long last_compile_time;
    pthread_t thread;
    t_sim *sim;
    pthread_mutex_t mutex;
} t_coder;

typedef struct s_sim {
    t_params params;
    t_coder *coders;
    t_dongle *dongles;
    int stop;
    long start_time;
    pthread_t monitor;
    pthread_mutex_t log_mutex;
    int log_mutex_initialized;
    pthread_mutex_t stop_mutex;
    int stop_mutex_initialized;
} t_sim;

int	is_valid_number(char *str);
int	is_valid_scheduler(char *str);
int init_sim(t_sim *sim, char **av);
void cleanup_sim(t_sim *sim);
long get_time_ms(void);
void sleep_ms(long ms);
int sim_should_stop(t_sim *sim);
void sim_set_stop(t_sim *sim, int value);
int coder_get_nb_compiles(t_coder *coder);
void coder_inc_nb_compiles(t_coder *coder);
long coder_get_last_compile_time(t_coder *coder);
void coder_set_last_compile_time(t_coder *coder, long time);
void log_state(t_sim *sim, int id, char *msg);
void heap_push(t_dongle *dongle, t_request req, char *scheduler);
void heap_pop(t_dongle *dongle, char *scheduler);
t_request heap_peek(t_dongle *dongle);
void dongle_acquire(t_sim *sim, int coder_id, int dongle_id);
void dongle_release(t_sim *sim, int dongle_id);
void *coder_routine(void *arg);
void *monitor_routine(void *arg);

#endif
