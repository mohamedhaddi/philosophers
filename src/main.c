/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 15:44:50 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 13:16:47 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

bool	all(bool *arr, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (arr[i] == false)
			return (false);
		i++;
	}
	return (true);
}

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_usleep(size_t time_in_ms)
{
	size_t	start_time;

	start_time = get_time();
	usleep((time_in_ms - 10) * 1000);
	while ((get_time() - time_in_ms) < start_time)
		;
}

void	pick_forks(pthread_mutex_t *forks_locks,
				t_thread_data *philosopher_data,
				size_t start_time)
{
	int	philosopher_number;
	int	number_of_philosophers;

	philosopher_number = philosopher_data->thread_number;
	number_of_philosophers
		= philosopher_data->input_data.number_of_philosophers;
	pthread_mutex_lock(&forks_locks[philosopher_number]);
	pthread_mutex_lock(&philosopher_data->locks->print_lock);
	printf("%zu	%d has taken a fork\n",
		get_time() - start_time,
		philosopher_number + 1);
	pthread_mutex_unlock(&philosopher_data->locks->print_lock);
	pthread_mutex_lock(
		&forks_locks[(philosopher_number + 1) % number_of_philosophers]);
	pthread_mutex_lock(&philosopher_data->locks->print_lock);
	printf("%zu	%d has taken a fork\n",
		get_time() - start_time,
		philosopher_number + 1);
	pthread_mutex_unlock(&philosopher_data->locks->print_lock);
}

void	eat(t_thread_data *philosopher_data, size_t start_time)
{
	int	philosopher_number;

	philosopher_number = philosopher_data->thread_number;
	pthread_mutex_lock(&philosopher_data->locks->state_lock);
	philosopher_data->state = EATING;
	pthread_mutex_unlock(&philosopher_data->locks->state_lock);
	pthread_mutex_lock(&philosopher_data->locks->latest_meal_time_lock);
	philosopher_data->latest_meal_time = get_time();
	pthread_mutex_unlock(&philosopher_data->locks->latest_meal_time_lock);
	pthread_mutex_lock(&philosopher_data->locks->state_lock);
	pthread_mutex_unlock(&philosopher_data->locks->state_lock);
	pthread_mutex_lock(&philosopher_data->locks->print_lock);
	printf("%zu	%d is eating\n",
		get_time() - start_time,
		philosopher_number + 1);
	pthread_mutex_unlock(&philosopher_data->locks->print_lock);
	pthread_mutex_lock(&philosopher_data->locks->total_meals_lock);
	if (philosopher_data->total_meals.is_set)
		philosopher_data->total_meals.value--;
	pthread_mutex_unlock(&philosopher_data->locks->total_meals_lock);
	ft_usleep(philosopher_data->input_data.time_to_eat);
}

void	release_forks(pthread_mutex_t *forks_locks,
					t_thread_data *philosopher_data)
{
	int	philosopher_number;
	int	number_of_philosophers;

	philosopher_number = philosopher_data->thread_number;
	number_of_philosophers
		= philosopher_data->input_data.number_of_philosophers;
	pthread_mutex_unlock(&forks_locks[philosopher_number]);
	pthread_mutex_unlock(
		&forks_locks[(philosopher_number + 1) % number_of_philosophers]);
}

void	take_a_nap(t_thread_data *philosopher_data, size_t start_time)
{
	int	philosopher_number;

	philosopher_number = philosopher_data->thread_number;
	pthread_mutex_lock(&philosopher_data->locks->state_lock);
	philosopher_data->state = SLEEPING;
	pthread_mutex_unlock(&philosopher_data->locks->state_lock);
	pthread_mutex_lock(&philosopher_data->locks->print_lock);
	printf("%zu	%d is sleeping\n",
		get_time() - start_time,
		philosopher_number + 1);
	pthread_mutex_unlock(&philosopher_data->locks->print_lock);
	ft_usleep(philosopher_data->input_data.time_to_sleep);
}

void	think(t_thread_data *philosopher_data, size_t start_time)
{
	int	philosopher_number;

	philosopher_number = philosopher_data->thread_number;
	pthread_mutex_lock(&philosopher_data->locks->state_lock);
	philosopher_data->state = THINKING;
	pthread_mutex_unlock(&philosopher_data->locks->state_lock);
	pthread_mutex_lock(&philosopher_data->locks->print_lock);
	printf("%zu	%d is thinking\n",
		get_time() - start_time,
		philosopher_number + 1);
	pthread_mutex_unlock(&philosopher_data->locks->print_lock);
}

void	set_thread_start_time(t_thread_data *philosopher_data)
{
	pthread_mutex_lock(&philosopher_data->locks->start_time_lock);
	pthread_mutex_lock(&philosopher_data->locks->latest_meal_time_lock);
	philosopher_data->start_time = get_time();
	philosopher_data->latest_meal_time = philosopher_data->start_time;
	pthread_mutex_unlock(&philosopher_data->locks->latest_meal_time_lock);
	pthread_mutex_unlock(&philosopher_data->locks->start_time_lock);
}

void	*philosopher_routine(void *thread_data)
{
	t_thread_data	*philosopher_data;
	size_t			start_time;
	pthread_mutex_t	*forks_locks;

	philosopher_data = (t_thread_data *)thread_data;
	set_thread_start_time(philosopher_data);
	start_time = philosopher_data->start_time;
	forks_locks = philosopher_data->locks->forks;
	if (philosopher_data->thread_number % 2)
		ft_usleep(philosopher_data->input_data.time_to_eat);
	while (1)
	{
		pick_forks(forks_locks, philosopher_data, start_time);
		eat(philosopher_data, start_time);
		release_forks(forks_locks, philosopher_data);
		take_a_nap(philosopher_data, start_time);
		think(philosopher_data, start_time);
	}
	return (NULL);
}

bool	all_philosophers_satiated(int i,
								bool *philosophers_satiated,
								t_thread_data *philosophers_data,
								t_data data)
{
	pthread_mutex_lock(&philosophers_data[i].locks->total_meals_lock);
	if (philosophers_data[i].total_meals.value == 0)
	{
		philosophers_satiated[i] = true;
		if (all(philosophers_satiated, data.number_of_philosophers))
			return (true);
	}
	pthread_mutex_unlock(&philosophers_data[i].locks->total_meals_lock);
	return (false);
}

bool	a_philosopher_died(int i, t_thread_data *philosophers_data, t_data data)
{
	pthread_mutex_lock(&philosophers_data[i].locks->state_lock);
	pthread_mutex_lock(&philosophers_data[i].locks->latest_meal_time_lock);
	pthread_mutex_lock(&philosophers_data[i].locks->print_lock);
	if ((philosophers_data[i].state != EATING
			&& (get_time() - philosophers_data[i].latest_meal_time)
			>= (size_t)data.time_to_die))
	{
		pthread_mutex_lock(&philosophers_data[i].locks->start_time_lock);
		printf("%zu	%d died\n",
			get_time() - philosophers_data[i].start_time,
			i + 1);
		return (true);
	}
	pthread_mutex_unlock(&philosophers_data[i].locks->print_lock);
	pthread_mutex_unlock(&philosophers_data[i].locks->state_lock);
	pthread_mutex_unlock(&philosophers_data[i].locks->latest_meal_time_lock);
	return (false);
}

void	*panic(char *error_message)
{
	printf("%s\n", error_message);
	return (NULL);
}

bool	*supervise(t_data data, t_thread_data *philosophers_data)
{
	bool	*philosophers_satiated;
	int		i;

	philosophers_satiated = malloc(sizeof(bool) * data.number_of_philosophers);
	if (!philosophers_satiated)
		return (panic("Failed to malloc philosophers_satiated array."));
	memset(philosophers_satiated,
		false,
		sizeof(bool) * data.number_of_philosophers);
	ft_usleep(20);
	i = 0;
	while (1)
	{
		if (a_philosopher_died(i, philosophers_data, data))
			break ;
		if (data.total_meals.is_set)
			if (all_philosophers_satiated(
					i, philosophers_satiated, philosophers_data, data))
				break ;
		i = (i + 1) % data.number_of_philosophers;
		usleep(200);
	}
	return (philosophers_satiated);
}

void	destroy_mutexes(t_locks *locks, t_data data)
{
	int	i;

	i = 0;
	while (i < data.number_of_forks)
	{
		pthread_mutex_destroy(&locks->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&locks->print_lock);
	pthread_mutex_destroy(&locks->start_time_lock);
	pthread_mutex_destroy(&locks->latest_meal_time_lock);
	pthread_mutex_destroy(&locks->state_lock);
	pthread_mutex_destroy(&locks->total_meals_lock);
}

void	*create_threads(pthread_t *philosophers,
						t_thread_data *philosophers_data,
						t_data data,
						t_locks *locks)
{
	int	i;

	i = 0;
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
		if (pthread_create(&philosophers[i],
				NULL,
				philosopher_routine,
				&philosophers_data[i]))
			return (NULL);
		i++;
	}
	return (philosophers);
}

t_locks	*init_locks(t_locks *locks, t_data data)
{
	int	i;

	i = 0;
	while (i < data.number_of_forks)
	{
		if (pthread_mutex_init(&locks->forks[i], NULL))
			return (NULL);
		i++;
	}
	if (pthread_mutex_init(&locks->print_lock, NULL)
		|| pthread_mutex_init(&locks->start_time_lock, NULL)
		|| pthread_mutex_init(&locks->latest_meal_time_lock, NULL)
		|| pthread_mutex_init(&locks->state_lock, NULL)
		|| pthread_mutex_init(&locks->total_meals_lock, NULL))
		return (NULL);
	return (locks);
}

int	free_allocated_memory(t_locks *locks,
							t_threads *philosophers,
							bool *philosophers_satiated,
							t_data *data)
{
	if (data)
		free(data);
	if (locks)
	{
		free(locks->forks);
		free(locks);
	}
	if (philosophers)
	{
		free(philosophers->threads);
		free(philosophers->threads_data);
		free(philosophers);
	}
	if (philosophers_satiated)
		free(philosophers_satiated);
	return (EXIT_FAILURE);
}

t_locks	*create_locks(t_data data)
{
	t_locks	*locks;

	locks = malloc(sizeof(t_locks));
	if (!locks)
		return (panic("Failed to malloc locks."));
	locks->forks = malloc(sizeof(pthread_mutex_t) * data.number_of_forks);
	if (!locks->forks)
		return (panic("Failed to malloc forks."));
	if (!init_locks(locks, data))
		return (panic("Locks initialization failed."));
	return (locks);
}

t_threads	*create_philosophers(t_locks *locks, t_data data)
{
	t_threads	*philosophers;

	philosophers = malloc(sizeof(t_threads));
	if (!philosophers)
		return (panic("Failed to malloc for philosophers."));
	philosophers->threads
		= malloc(sizeof(pthread_t) * data.number_of_philosophers);
	if (!philosophers->threads)
	{
		free(philosophers);
		return (panic("Failed to malloc for philosophers' threads."));
	}
	philosophers->threads_data
		= malloc(sizeof(t_thread_data) * data.number_of_philosophers);
	if (!philosophers->threads_data)
	{
		free(philosophers->threads);
		free(philosophers);
		return (panic("Failed to malloc for philosophers' data."));
	}
	if (!create_threads(
			philosophers->threads, philosophers->threads_data, data, locks))
		return (panic("Threads creation failed."));
	return (philosophers);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_threads	*philosophers;
	bool		*philosophers_satiated;
	t_locks		*locks;

	data = get_data(argc, argv);
	if (!data)
		return (EXIT_FAILURE);
	locks = create_locks(*data);
	if (!locks)
		return (free_allocated_memory(NULL, NULL, NULL, data));
	philosophers = create_philosophers(locks, *data);
	if (!philosophers)
		return (free_allocated_memory(locks, NULL, NULL, data));
	philosophers_satiated = supervise(*data, philosophers->threads_data);
	if (!philosophers_satiated)
		return (free_allocated_memory(locks, philosophers, NULL, data));
	destroy_mutexes(locks, *data);
	free_allocated_memory(locks, philosophers, philosophers_satiated, data);
	system("leaks philo");
	return (EXIT_SUCCESS);
}
