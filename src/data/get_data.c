/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 18:22:54 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 12:50:12 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

bool	is_valid_data(t_data data)
{
    if (data.number_of_philosophers <= 0)
        return (false);
    if (data.time_to_die < 0)
        return (false);
    if (data.time_to_eat < 0)
        return (false);
    if (data.time_to_sleep < 0)
        return (false);
    if (data.total_meals.value < 0)
        return (false);
    return (true);
}

t_data	get_input(char **argv, int argc)
{
	t_data	data;

	data.number_of_philosophers = ft_atoi(argv[1]);
	data.number_of_forks = data.number_of_philosophers;
	data.time_to_die = ft_atoi(argv[2]);
	data.time_to_eat = ft_atoi(argv[3]);
	data.time_to_sleep = ft_atoi(argv[4]);
	data.total_meals.is_set = (argc == 6);
	data.total_meals.value = INIT_WITH_ZERO;
	if (data.total_meals.is_set)
		data.total_meals.value = ft_atoi(argv[5]);
	return (data);
}

int	are_valid_args(char **argv, int argc)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (false);
	i = 1;
	while (i < argc)
	{
		if (is_over_int(argv[i]) || !is_integer(argv[i]))
			return (false);
		i++;
	}
	return (true);
}

t_data	*get_data(int argc, char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!are_valid_args(argv, argc))
		return (panic("Invalid arguments."));
	*data = get_input(argv, argc);
	if (!is_valid_data(*data))
		return (panic("Invalid data."));
	return (data);
}