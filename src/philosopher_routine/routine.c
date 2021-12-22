/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 16:21:34 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:55:17 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

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
