/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release_forks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 16:22:25 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:55:01 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/philo.h"

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
