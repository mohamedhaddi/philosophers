/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 16:21:12 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:54:25 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

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
