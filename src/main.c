/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 15:44:50 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 17:00:11 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

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
	return (EXIT_SUCCESS);
}
