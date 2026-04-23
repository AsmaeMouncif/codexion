/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asmounci <asmounci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 15:08:00 by asmounci          #+#    #+#             */
/*   Updated: 2026/04/23 16:15:20 by asmounci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <unistd.h>
# include <stdlib.h>
# include <string.h>

int	is_valid_number(char *str);
int	is_valid_scheduler(char *str);

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
    struct s_request *next;
} t_request;


typedef struct s_dongle {
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    long last_release_time;
    t_request *queue;
} t_dongle;

typedef struct s_coder {
        

    
} t_coder;


#endif