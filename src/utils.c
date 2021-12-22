/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 16:17:44 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:56:55 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*panic(char *error_message)
{
	printf("%s\n", error_message);
	return (NULL);
}

bool	all(bool *arr, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if (arr[i] == false)
			return (false);
		i++;
	}
	return (true);
}

size_t	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	ft_usleep(size_t time_in_ms)
{
	size_t	start_time;

	start_time = get_time();
	usleep((time_in_ms - 10) * 1000);
	while ((get_time() - time_in_ms) < start_time)
		;
}

int	free_allocated_memory(t_locks *locks,
							t_threads *philosophers,
							bool *philosophers_satiated,
							t_data *data)
{
	if (data)
		free(data);
	if (locks)
	{
		free(locks->forks);
		free(locks);
	}
	if (philosophers)
	{
		free(philosophers->threads);
		free(philosophers->threads_data);
		free(philosophers);
	}
	if (philosophers_satiated)
		free(philosophers_satiated);
	return (EXIT_FAILURE);
}
