/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 15:44:50 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/19 15:48:41 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <pthread.h>
#include <string.h>

void print_data(t_data data)
{
	printf("n_of_philos: %d\n", data.number_of_philosophers);
	printf("time_to_die: %d\n", data.time_to_die);
	printf("time_to_eat: %d\n", data.time_to_eat);
	printf("time_to_sleep: %d\n", data.time_to_sleep);
	printf("total_meals value: %d\n", data.total_meals.value);
	printf("total_meals is_set: %d\n", data.total_meals.is_set);
	exit(0);
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

void	ft_usleep(size_t time_in_ms)
{
	size_t start_time;

	start_time = 0;
	start_time = get_time();
	while ((get_time() - start_time) < time_in_ms)
		usleep(time_in_ms / 10);
}

void *philosopher_routine(void *thread_data)
{
	t_thread_data *philosopher_data;
	int number_of_philosophers;
	int philosopher_number;
	int time_to_die;
	size_t start_time;
	pthread_mutex_t *forks_locks;

	philosopher_data = (t_thread_data *)thread_data;
	number_of_philosophers = philosopher_data->input_data.number_of_philosophers;
	philosopher_number = philosopher_data->thread_number;
	forks_locks = philosopher_data->locks.forks;
	time_to_die = philosopher_data->input_data.time_to_die;

	// get starting time:
	pthread_mutex_lock(&philosopher_data->locks.start_time_lock);
	pthread_mutex_lock(&philosopher_data->locks.latest_meal_time_lock);
	philosopher_data->start_time = get_time();
	start_time = philosopher_data->start_time;
	philosopher_data->latest_meal_time = start_time;
	pthread_mutex_unlock(&philosopher_data->locks.latest_meal_time_lock);
	pthread_mutex_unlock(&philosopher_data->locks.start_time_lock);

	if (philosopher_number % 2)
		ft_usleep(philosopher_data->input_data.time_to_eat);

	while (1)
	{
		// pick forks
		pthread_mutex_lock(&forks_locks[philosopher_number]);
		pthread_mutex_lock(&philosopher_data->locks.print_lock);
		printf("%zu %d has taken a fork\n", get_time() - start_time, philosopher_number + 1);
		pthread_mutex_unlock(&philosopher_data->locks.print_lock);
		pthread_mutex_lock(&forks_locks[(philosopher_number + 1) % number_of_philosophers]);
		pthread_mutex_lock(&philosopher_data->locks.print_lock);
		printf("%zu %d has taken a fork\n", get_time() - start_time, philosopher_number + 1);
		pthread_mutex_unlock(&philosopher_data->locks.print_lock);

		// eat
		pthread_mutex_lock(&philosopher_data->locks.state_lock);
		philosopher_data->state = READY_TO_EAT;
		pthread_mutex_unlock(&philosopher_data->locks.state_lock);
		pthread_mutex_lock(&philosopher_data->locks.latest_meal_time_lock);
		philosopher_data->latest_meal_time = get_time();
		pthread_mutex_unlock(&philosopher_data->locks.latest_meal_time_lock);
		pthread_mutex_lock(&philosopher_data->locks.state_lock);
		philosopher_data->state = EATING;
		pthread_mutex_unlock(&philosopher_data->locks.state_lock);
		pthread_mutex_lock(&philosopher_data->locks.print_lock);
		printf("%zu %d is eating\n", get_time() - start_time, philosopher_number + 1);
		// decrement total meals
		pthread_mutex_lock(&philosopher_data->locks.total_meals_lock);
		if (philosopher_data->total_meals.is_set)
		{
			philosopher_data->total_meals.value--;
			printf("philo %d total meals: %d\n", philosopher_number, philosopher_data->total_meals.value);
		}
		pthread_mutex_unlock(&philosopher_data->locks.total_meals_lock);
		pthread_mutex_unlock(&philosopher_data->locks.print_lock);
		ft_usleep(philosopher_data->input_data.time_to_eat);

		// release forks
		pthread_mutex_unlock(&forks_locks[philosopher_number]);
		pthread_mutex_unlock(&forks_locks[(philosopher_number + 1) % number_of_philosophers]);

		// sleep
		pthread_mutex_lock(&philosopher_data->locks.state_lock);
		philosopher_data->state = SLEEPING;
		pthread_mutex_unlock(&philosopher_data->locks.state_lock);
		pthread_mutex_lock(&philosopher_data->locks.print_lock);
		printf("%zu %d is sleeping\n", get_time() - start_time, philosopher_number + 1);
		pthread_mutex_unlock(&philosopher_data->locks.print_lock);
		ft_usleep(philosopher_data->input_data.time_to_sleep);

		// think
		pthread_mutex_lock(&philosopher_data->locks.state_lock);
		philosopher_data->state = THINKING;
		pthread_mutex_unlock(&philosopher_data->locks.state_lock);
		pthread_mutex_lock(&philosopher_data->locks.print_lock);
		printf("%zu %d is thinking\n", get_time() - start_time, philosopher_number + 1);
		pthread_mutex_unlock(&philosopher_data->locks.print_lock);
	}

	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data data;
	t_thread_data *philosophers_data;
	pthread_t *philosophers;
	t_locks locks;

	// check input
    if (!are_valid_args(argv, argc))
        return (EXIT_FAILURE);
	data = get_data(argv, argc);
	if (!is_valid_data(data))
		return (EXIT_FAILURE);

	// print_data(data);

	// malloc threads array for philosophers
	philosophers = malloc(sizeof(pthread_t) * data.number_of_philosophers);

	// create mutex locks
	locks.forks = malloc(sizeof(pthread_mutex_t) * data.number_of_forks);
	int i = 0;
	while (i < data.number_of_forks)
	{
		pthread_mutex_init(&locks.forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&locks.print_lock, NULL);
	pthread_mutex_init(&locks.start_time_lock, NULL);
	pthread_mutex_init(&locks.latest_meal_time_lock, NULL);
	pthread_mutex_init(&locks.state_lock, NULL);
	pthread_mutex_init(&locks.total_meals_lock, NULL);

	// create philosophers threads
	i = 0;
	philosophers_data = malloc(sizeof(t_thread_data) * data.number_of_philosophers);
	while (i < data.number_of_philosophers)
	{
		philosophers_data[i].thread = philosophers[i];
		philosophers_data[i].total_meals = data.total_meals;
		philosophers_data[i].locks = locks;
		philosophers_data[i].input_data = data;
		philosophers_data[i].thread_number = i;
		philosophers_data[i].state = THINKING;
		philosophers_data[i].start_time = INIT_WITH_ZERO;
		philosophers_data[i].latest_meal_time = INIT_WITH_ZERO;
		pthread_create(&philosophers[i], NULL, philosopher_routine, &philosophers_data[i]);
		i++;
	}

	/* death listener */

	// add delay until all philosophers start (time_to_eat)
	ft_usleep(data.time_to_eat);
	int number_of_satiated_philosophers = 0;
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&philosophers_data[i].locks.state_lock);
		pthread_mutex_lock(&philosophers_data[i].locks.latest_meal_time_lock);
		if ((philosophers_data[i].state != READY_TO_EAT
			&& (get_time() - philosophers_data[i].latest_meal_time) >= (size_t)data.time_to_die))
		{
			pthread_mutex_lock(&philosophers_data[i].locks.start_time_lock); // may not need this lock thanks to the delay
			pthread_mutex_lock(&philosophers_data[i].locks.print_lock);
			printf("%zu %d died\n", get_time() - philosophers_data[i].start_time, i + 1);
			/*
			pthread_mutex_unlock(&philosophers_data[i].locks.start_time_lock);
			pthread_mutex_unlock(&philosophers_data[i].locks.state_lock);
			pthread_mutex_unlock(&philosophers_data[i].locks.latest_meal_time_lock);
			*/
			break ;
		}
		pthread_mutex_unlock(&philosophers_data[i].locks.state_lock);
		pthread_mutex_unlock(&philosophers_data[i].locks.latest_meal_time_lock);

		pthread_mutex_lock(&philosophers_data[i].locks.total_meals_lock);
		if (philosophers_data[i].total_meals.is_set && philosophers_data[i].total_meals.value == 0)
		{
			number_of_satiated_philosophers++;
			if (number_of_satiated_philosophers == data.number_of_philosophers)
			{
				int j = 0;
				pthread_mutex_lock(&philosophers_data[i].locks.print_lock);
				while (j < data.number_of_philosophers)
				{
					printf("philo %d total meals: %d\n", j, philosophers_data[i].total_meals.value);
					j++;
				}
				break ;
			}
		}
		pthread_mutex_unlock(&philosophers_data[i].locks.total_meals_lock);

		i = (i + 1) % data.number_of_philosophers;
	}

	/* free all */

	/*
	i = 0;
	while (i < data.number_of_philosophers)
	{
		pthread_detach(philosophers[i]);
		i++;
	}
	i = 0;
	while (i < data.number_of_forks)
	{
		pthread_mutex_destroy(&locks.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&locks.print_lock);
	pthread_mutex_destroy(&locks.start_time_lock);
	pthread_mutex_destroy(&locks.latest_meal_time_lock);
	pthread_mutex_destroy(&locks.state_lock);
	*/

	// don't forget to free allocated memory

	return (EXIT_SUCCESS);
}
