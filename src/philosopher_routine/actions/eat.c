/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 16:22:32 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:55:08 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/philo.h"

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
