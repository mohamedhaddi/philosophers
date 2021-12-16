/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 15:44:50 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/16 13:41:00 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <string.h>
#include <pthread.h>

void print_data(t_data data)
{
	printf("%d\n", data.number_of_philosophers);
	printf("%d\n", data.time_to_die);
	printf("%d\n", data.time_to_eat);
	printf("%d\n", data.time_to_sleep);
	printf("%d\n", data.number_of_meals.value);
	printf("%d\n", data.number_of_meals.is_set);
}

void init_locks(pthread_mutex_t *locks, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		pthread_mutex_init(&locks[i], NULL);
		i++;
	}
}

void create_threads(pthread_t *threads, int size, void *(*start_routine)(void *), void *routine_data)
{
	int i;
	t_routine_data *routines_data_arr;

	i = 0;
	routines_data_arr = malloc(sizeof(t_routine_data) * size);
	while (i < size)
	{
		memcpy(&routines_data_arr[i], routine_data, sizeof(t_routine_data)); // to replace with ft_memcpy
		routines_data_arr[i].thread_number = i;
		pthread_create(&threads[i], NULL, start_routine, &routines_data_arr[i]);
		i++;
	}
}

void join_multiple_threads(pthread_t *threads, int size)
{
	int i;

	i = 0;
	while (i < size)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

void *philosopher_routine(void *routine_data)
{
	t_routine_data *philo_routine_data;
	int number_of_philosophers;
	int philosopher_number;
	pthread_mutex_t *forks;

	philo_routine_data = (t_routine_data *)routine_data;
	number_of_philosophers = philo_routine_data->number_of_threads;
	philosopher_number = philo_routine_data->thread_number;
	forks = philo_routine_data->locks;

	printf("%d\n", philosopher_number);
	/*
	// think
	printf("timestamp_in_ms %d is thinking\n", philosopher_number);

	// take fork
	pthread_mutex_lock(&forks[philosopher_number]);
	printf("timestamp_in_ms %d has taken a fork\n", philosopher_number);
	pthread_mutex_lock(&forks[(philosopher_number + 1) % number_of_philosophers]);
	printf("timestamp_in_ms %d has taken a fork\n", philosopher_number);

	// eat
	printf("timestamp_in_ms %d is eating\n", philosopher_number);
	sleep(3); // use usleep time_to_eat

	// sleep
	printf("timestamp_in_ms %d is sleeping\n", philosopher_number);
	sleep(3); // use usleep time_to_sleep

	// release forks
	pthread_mutex_unlock(&forks[philosopher_number]);
	pthread_mutex_unlock(&forks[(philosopher_number + 1) % number_of_philosophers]);

	printf("timestamp_in_ms %d finished eating\n", philosopher_number);
	*/

	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data data;
	t_routine_data philo_routine_data;
	pthread_t *philosophers;
	pthread_mutex_t *forks;

    if (!are_valid_args(argv, argc))
        return (EXIT_FAILURE);
	data = get_data(argv, argc);
	if (!is_valid_data(data))
		return (EXIT_FAILURE);

	// print_data(data);

	// malloc threads array for philosophers
	philosophers = malloc(sizeof(pthread_t) * data.number_of_philosophers);

	// malloc locks array for forks
	forks = malloc(sizeof(pthread_mutex_t) * data.number_of_forks);

	// create forks locks (mutex locks)
	init_locks(forks, data.number_of_forks);

	// create philosophers threads
	philo_routine_data.threads = philosophers;
	philo_routine_data.locks = forks;
	philo_routine_data.number_of_threads = data.number_of_philosophers;
	create_threads(
			philosophers,
			data.number_of_philosophers,
			philosopher_routine,
			&philo_routine_data);

	// make this calling thread wait for the termination of the philosophers threads
	join_multiple_threads(philosophers, data.number_of_philosophers);

	return (EXIT_SUCCESS);
}
