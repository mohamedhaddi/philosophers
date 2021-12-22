/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 18:20:43 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:21:07 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H
# define INIT_WITH_ZERO 0

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

enum e_state
{
	THINKING,
	EATING,
	SLEEPING,
	DEAD
};

typedef struct s_optional_arg
{
	bool			is_set;
	int				value;
}					t_optional_arg;

typedef struct s_data
{
	int				number_of_philosophers;
	int				number_of_forks;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	t_optional_arg	total_meals;
}					t_data;

typedef struct s_locks
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	start_time_lock;
	pthread_mutex_t	latest_meal_time_lock;
	pthread_mutex_t	state_lock;
	pthread_mutex_t	total_meals_lock;
}					t_locks;

typedef struct s_thread_data
{
	t_data			input_data;
	t_optional_arg	total_meals;
	pthread_t		thread;
	t_locks			*locks;
	int				thread_number;
	size_t			start_time;
	size_t			latest_meal_time;
	int				state;
}					t_thread_data;

typedef struct s_threads
{
	pthread_t		*threads;
	t_thread_data	*threads_data;
}					t_threads;

int					are_valid_args(char **argv, int argc);
t_data				*get_data(int argc, char **argv);
t_data				get_input(char **argv, int argc);
bool				is_valid_data(t_data data);
void				*panic(char *error_message);
int					ft_isdigit(int c);
bool				is_integer(char *str);
size_t				ft_strlen(const char *s);
int					ft_atoi(const char *str);
bool				is_over_int(char *str);
t_threads			*create_philosophers(t_locks *locks, t_data data);
void				*philosopher_routine(void *thread_data);
bool				*supervise(t_data data, t_thread_data *philosophers_data);
void				pick_forks(pthread_mutex_t *forks_locks,
					t_thread_data *philosopher_data,
					size_t start_time);
void				eat(t_thread_data *philosopher_data, size_t start_time);
void				release_forks(pthread_mutex_t *forks_locks,
					t_thread_data *philosopher_data);
void				take_a_nap(t_thread_data *philosopher_data, size_t start_time);
void				think(t_thread_data *philosopher_data, size_t start_time);
t_locks				*create_locks(t_data data);
void				destroy_mutexes(t_locks *locks, t_data data);
int					free_allocated_memory(t_locks *locks,
					t_threads *philosophers,
					bool *philosophers_satiated,
					t_data *data);
bool				all(bool *arr, size_t len);
void				ft_usleep(size_t time_in_ms);
size_t				get_time(void);

#endif
