/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 15:44:50 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/17 18:27:53 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <string.h>

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

size_t	get_time(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void *philosopher_routine(void *thread_data)
{
	t_thread_data *philosopher_data;
	int number_of_philosophers;
	int philosopher_number;
	int time_to_die;
	pthread_mutex_t *forks;

	philosopher_data = (t_thread_data *)thread_data;
	number_of_philosophers = philosopher_data->input_data.number_of_philosophers;
	philosopher_number = philosopher_data->thread_number;
	forks = philosopher_data->locks.forks;
	time_to_die = philosopher_data->input_data.time_to_die;

	// get starting time:
	philosopher_data->start_time = get_time();

	// think:
	printf("timestamp_in_ms %d is thinking\n", philosopher_number);
	// pick forks:
	if (philosopher_number % 2)
	{
		pthread_mutex_lock(&forks[(philosopher_number + 1) % number_of_philosophers]);
		printf("timestamp_in_ms %d has taken a fork\n", philosopher_number);
		pthread_mutex_lock(&forks[philosopher_number]);
		printf("timestamp_in_ms %d has taken a fork\n", philosopher_number);
	}
	else
	{
		pthread_mutex_lock(&forks[philosopher_number]);
		printf("timestamp_in_ms %d has taken a fork\n", philosopher_number);
		pthread_mutex_lock(&forks[(philosopher_number + 1) % number_of_philosophers]);
		printf("timestamp_in_ms %d has taken a fork\n", philosopher_number);
	}

	// eat:
	printf("timestamp_in_ms %d is eating\n", philosopher_number);
	// usleep time_to_eat...
	// release forks:
	pthread_mutex_unlock(&forks[philosopher_number]);
	pthread_mutex_unlock(&forks[(philosopher_number + 1) % number_of_philosophers]);
	// reset starting time:
	philosopher_data->start_time = get_time();

	// sleep:
	printf("timestamp_in_ms %d is sleeping\n", philosopher_number);
	// usleep time_to_sleep...
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data data;
	t_thread_data *philosophers_data;
	pthread_t *philosophers;
	pthread_mutex_t *forks;

    if (!are_valid_args(argv, argc))
        return (EXIT_FAILURE);
	data = get_data(argv, argc);
	if (!is_valid_data(data))
		return (EXIT_FAILURE);

	// malloc threads array for philosophers
	philosophers = malloc(sizeof(pthread_t) * data.number_of_philosophers);

	// malloc locks array for forks
	forks = malloc(sizeof(pthread_mutex_t) * data.number_of_forks);

	// create forks locks (mutex locks)
	int i = 0;
	while (i < data.number_of_forks)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}

	// create philosophers threads
	i = 0;
	philosophers_data = malloc(sizeof(t_thread_data) * data.number_of_philosophers);
	while (i < data.number_of_philosophers)
	{
		philosophers_data[i].input_data = data;
		philosophers_data[i].thread = philosophers[i];
		philosophers_data[i].locks.forks = forks;
		philosophers_data[i].thread_number = i;
		philosophers_data[i].state = THINKING;
		philosophers_data[i].start_time = INIT_WITH_ZERO;
		pthread_create(&philosophers[i], NULL, philosopher_routine, &philosophers_data[i]);
		i++;
	}

	// death listener
	i = 0;
	while (1)	
	{
		if ((get_time() - philosophers_data[i].start_time) > data.time_to_die)
		{
			printf("timestamp_in_ms %d died\n", philosophers_data[i].thread_number);
			// stop simulation!
		}
		i = (i + 1) % data.number_of_philosophers;
	}

	// make this calling thread wait for the termination of the philosophers threads
	// join_multiple_threads(philosophers, data.number_of_philosophers);

	return (EXIT_SUCCESS);
}
