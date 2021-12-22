/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy_locks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 16:22:42 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:54:32 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

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
