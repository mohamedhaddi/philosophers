/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pick_forks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 16:22:28 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:54:58 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/philo.h"

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
