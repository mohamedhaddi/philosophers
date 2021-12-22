/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   supervise.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 16:21:21 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:54:40 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

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
