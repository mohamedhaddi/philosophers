/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 15:44:50 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/15 15:36:51 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
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

void create_locks(pthread_mutex_t *locks, int size)
{
	int i;

	locks = malloc(sizeof(pthread_mutex_t) * size);
	i = 0;
	while (i < size)
	{
		pthread_mutex_init(&locks[i], NULL);
		i++;
	}
}

void create_threads(pthread_t *threads, int size, void *(*start_routine)(void *), void *arg)
{
	int i;

	threads = malloc(sizeof(pthread_t) * size);
	i = 0;
	while (i < size)
	{
		pthread_create(&threads[size], NULL, start_routine, NULL);
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

void *philosopher_lifetime(void *arg)
{
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data data;
	pthread_t *philosophers;
	pthread_mutex_t *forks;

    if (!are_valid_args(argv, argc))
        return (EXIT_FAILURE);
	data = get_data(argv, argc);
	if (!is_valid_data(data))
		return (EXIT_FAILURE);

	// print_data(data);

	// create locks for forks (mutex locks)
	create_locks(forks, data.number_of_forks);

	// create philosophers threads
	create_threads(philosophers, data.number_of_philosophers, philosopher_lifetime, NULL);

	// make this calling thread wait for the termination of the philosophers threads
	join_multiple_threads(philosophers, data.number_of_philosophers);

	return (EXIT_SUCCESS);
}
