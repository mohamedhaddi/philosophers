/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 15:44:50 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/18 18:32:56 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"
#include <pthread.h>
#include <string.h>

void print_data(t_data data)
{
	printf("%d\n", data.number_of_philosophers);
	printf("%d\n", data.time_to_die);
	printf("%d\n", data.time_to_eat);
	printf("%d\n", data.time_to_sleep);
	printf("%d\n", data.total_meals.value);
	printf("%d\n", data.total_meals.is_set);
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
	pthread_mutex_t *forks_locks;

	philosopher_data = (t_thread_data *)thread_data;
	number_of_philosophers = philosopher_data->input_data.number_of_philosophers;
	philosopher_number = philosopher_data->thread_number;
	forks_locks = philosopher_data->locks.forks;
	time_to_die = philosopher_data->input_data.time_to_die;

	// get starting time:
	pthread_mutex_lock(philosopher_data->locks.start_time_lock);
	pthread_mutex_lock(philosopher_data->locks.latest_meal_time_lock);
	philosopher_data->start_time = get_time();
	philosopher_data->latest_meal_time = philosopher_data->start_time;
	pthread_mutex_unlock(philosopher_data->locks.latest_meal_time_lock);
	pthread_mutex_unlock(philosopher_data->locks.start_time_lock);

	// while (*philosopher_data->death == false)
	while (1)
	{
		// pick forks:
		if (philosopher_number % 2)
		{
			pthread_mutex_lock(&forks_locks[philosopher_number]);
			pthread_mutex_lock(philosopher_data->locks.print_lock);
			pthread_mutex_lock(philosopher_data->locks.start_time_lock);
			printf("%zu\t%d\thas taken a fork\n", get_time() - philosopher_data->start_time, philosopher_number + 1);
			pthread_mutex_unlock(philosopher_data->locks.start_time_lock);
			pthread_mutex_unlock(philosopher_data->locks.print_lock);
			pthread_mutex_lock(&forks_locks[(philosopher_number + 1) % number_of_philosophers]);
			pthread_mutex_lock(philosopher_data->locks.print_lock);
			pthread_mutex_lock(philosopher_data->locks.start_time_lock);
			printf("%zu\t%d\thas taken a fork\n", get_time() - philosopher_data->start_time, philosopher_number + 1);
			pthread_mutex_unlock(philosopher_data->locks.start_time_lock);
			pthread_mutex_unlock(philosopher_data->locks.print_lock);
		}
		else
		{
			pthread_mutex_lock(&forks_locks[(philosopher_number + 1) % number_of_philosophers]);
			pthread_mutex_lock(philosopher_data->locks.print_lock);
			pthread_mutex_lock(philosopher_data->locks.start_time_lock);
			printf("%zu\t%d\thas taken a fork\n", get_time() - philosopher_data->start_time, philosopher_number + 1);
			pthread_mutex_unlock(philosopher_data->locks.start_time_lock);
			pthread_mutex_unlock(philosopher_data->locks.print_lock);
			pthread_mutex_lock(&forks_locks[philosopher_number]);
			pthread_mutex_lock(philosopher_data->locks.print_lock);
			pthread_mutex_lock(philosopher_data->locks.start_time_lock);
			printf("%zu\t%d\thas taken a fork\n", get_time() - philosopher_data->start_time, philosopher_number + 1);
			pthread_mutex_unlock(philosopher_data->locks.start_time_lock);
			pthread_mutex_unlock(philosopher_data->locks.print_lock);
		}

		// eat:
		// reset starting time:
		pthread_mutex_lock(philosopher_data->locks.state_lock);
		philosopher_data->state = EATING;
		pthread_mutex_unlock(philosopher_data->locks.state_lock);
		pthread_mutex_lock(philosopher_data->locks.latest_meal_time_lock);
		philosopher_data->latest_meal_time = get_time();
		pthread_mutex_unlock(philosopher_data->locks.latest_meal_time_lock);
		pthread_mutex_lock(philosopher_data->locks.print_lock);
		pthread_mutex_lock(philosopher_data->locks.start_time_lock);
		printf("%zu\t%d\tis eating\n", get_time() - philosopher_data->start_time, philosopher_number + 1);
		pthread_mutex_unlock(philosopher_data->locks.start_time_lock);
		pthread_mutex_unlock(philosopher_data->locks.print_lock);
		// eat for time_to_eat ms
		ft_usleep(philosopher_data->input_data.time_to_eat);
		// decrement total meals
		/*
		if (philosopher_data->input_data.total_meals.is_set)
		{
			philosopher_data->input_data.total_meals.value--;
			if (philosopher_data->input_data.total_meals.value == 0)
				return (NULL);
		}
		*/
		// release forks:
		pthread_mutex_unlock(&forks_locks[philosopher_number]);
		pthread_mutex_unlock(&forks_locks[(philosopher_number + 1) % number_of_philosophers]);

		// sleep:
		pthread_mutex_lock(philosopher_data->locks.state_lock);
		philosopher_data->state = SLEEPING;
		pthread_mutex_unlock(philosopher_data->locks.state_lock);
		pthread_mutex_lock(philosopher_data->locks.print_lock);
		pthread_mutex_lock(philosopher_data->locks.start_time_lock);
		printf("%zu\t%d\tis sleeping\n", get_time() - philosopher_data->start_time, philosopher_number + 1);
		pthread_mutex_unlock(philosopher_data->locks.start_time_lock);
		pthread_mutex_unlock(philosopher_data->locks.print_lock);
		ft_usleep(philosopher_data->input_data.time_to_sleep);

		// think:
		pthread_mutex_lock(philosopher_data->locks.state_lock);
		philosopher_data->state = THINKING;
		pthread_mutex_unlock(philosopher_data->locks.state_lock);
		pthread_mutex_lock(philosopher_data->locks.print_lock);
		pthread_mutex_lock(philosopher_data->locks.start_time_lock);
		printf("%zu\t%d\tis thinking\n", get_time() - philosopher_data->start_time, philosopher_number + 1);
		pthread_mutex_unlock(philosopher_data->locks.start_time_lock);
		pthread_mutex_unlock(philosopher_data->locks.print_lock);
	}

	return (NULL);
}

int	main(int argc, char **argv)
{
	t_data data;
	t_thread_data *philosophers_data;
	pthread_t *philosophers;
	pthread_mutex_t *forks_locks;
	pthread_mutex_t print_lock;
	pthread_mutex_t start_time_lock;
	pthread_mutex_t latest_meal_time_lock;
	pthread_mutex_t state_lock;
	// bool death;

    if (!are_valid_args(argv, argc))
        return (EXIT_FAILURE);
	data = get_data(argv, argc);
	if (!is_valid_data(data))
		return (EXIT_FAILURE);

	// malloc threads array for philosophers
	philosophers = malloc(sizeof(pthread_t) * data.number_of_philosophers);

	// malloc locks array for forks
	forks_locks = malloc(sizeof(pthread_mutex_t) * data.number_of_forks);

	// create mutex locks:
	int i = 0;
	while (i < data.number_of_forks)
	{
		pthread_mutex_init(&forks_locks[i], NULL);
		i++;
	}
	pthread_mutex_init(&print_lock, NULL);
	pthread_mutex_init(&start_time_lock, NULL);
	pthread_mutex_init(&latest_meal_time_lock, NULL);
	pthread_mutex_init(&state_lock, NULL);

	// create philosophers threads
	i = 0;
	philosophers_data = malloc(sizeof(t_thread_data) * data.number_of_philosophers);
	while (i < data.number_of_philosophers)
	{
		philosophers_data[i].input_data = data;
		philosophers_data[i].thread = philosophers[i];
		philosophers_data[i].locks.forks = forks_locks;
		philosophers_data[i].locks.print_lock = &print_lock;
		philosophers_data[i].locks.start_time_lock = &start_time_lock;
		philosophers_data[i].locks.latest_meal_time_lock = &latest_meal_time_lock;
		philosophers_data[i].locks.state_lock = &state_lock;
		philosophers_data[i].thread_number = i;
		philosophers_data[i].start_time = get_time();
		philosophers_data[i].latest_meal_time = philosophers_data[i].start_time;
		philosophers_data[i].state = THINKING;
		// death = false;
		// philosophers_data[i].death = &death;
		pthread_create(&philosophers[i], NULL, philosopher_routine, &philosophers_data[i]);
		i++;
	}

	// death listener
	i = 0;
	while (1)
	{
		pthread_mutex_lock(&latest_meal_time_lock);
		if ((get_time() - philosophers_data[i].latest_meal_time) >= (size_t)data.time_to_die)
		{
			// death = true;
			pthread_mutex_lock(&start_time_lock);
			pthread_mutex_lock(&print_lock);
			printf("%zu\t%d\tdied\n", get_time() - philosophers_data[i].start_time, i + 1);
			break ;
		}
		pthread_mutex_unlock(&latest_meal_time_lock);
		i = (i + 1) % data.number_of_philosophers;
	}

	// free all
	i = 0;
	while (i < data.number_of_philosophers)
	{
		pthread_detach(philosophers[i]);
		i++;
	}
	i = 0;
	while (i < data.number_of_forks)
	{
		pthread_mutex_destroy(&forks_locks[i]);
		i++;
	}
	// don't forget to free allocated memory

	// make this calling thread wait for the termination of the philosophers threads
	// join_multiple_threads(philosophers, data.number_of_philosophers);

	return (EXIT_SUCCESS);
}
