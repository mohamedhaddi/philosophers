/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   think.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 16:22:19 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:54:47 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/philo.h"

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
