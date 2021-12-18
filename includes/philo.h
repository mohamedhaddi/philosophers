/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 18:20:43 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/18 12:27:29 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H
# define INIT_WITH_ZERO 0

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/time.h>

typedef	struct	s_optional_arg {
	bool is_set;
	int value;
}				t_optional_arg;

typedef	struct	s_data {
	int number_of_philosophers;
	int number_of_forks;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	t_optional_arg number_of_meals;
}				t_data;

typedef struct	s_locks {
	pthread_mutex_t *forks;
	pthread_mutex_t *print_status;
}				t_locks;

typedef struct	s_thread_data {
	t_data input_data;
	pthread_t thread;
	t_locks	locks;
	int	thread_number;
	size_t start_time;
	size_t last_meal_time;
	bool *death;
}				t_thread_data;

int are_valid_args(char **argv, int argc);
t_data get_data(char **argv, int argc);
bool is_valid_data(t_data data);

#endif
