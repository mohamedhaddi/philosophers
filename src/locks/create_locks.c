/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_locks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 16:22:44 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:54:36 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

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
