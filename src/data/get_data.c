/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_data.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 18:22:54 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/12 18:26:50 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

int ft_atoi(const char *str)
{
    int             j;
    unsigned long   res;
    int             temp;

    temp = 1;
    j = 0;
    res = 0;
    while (*str > 0 && *str <= 32)
    {
        if (*str == 27)
            return (0);
        str++;
    }
    if (*str == '-')
    {
        temp = -1;
        str++;
    }
    else if (*str == '+')
        str++;
    while (*(str + j) >= 48 && *(str + j) <= 57)
        res = res * 10 + *(str + j++) - '0';
    return (res * temp);
}

t_data get_data(char **argv, int argc)
{
	t_data data;

	data.number_of_philosophers = ft_atoi(argv[1]);
	data.number_of_forks = data.number_of_philosophers;
	data.time_to_die = ft_atoi(argv[2]);
	data.time_to_eat = ft_atoi(argv[3]);
	data.time_to_sleep = ft_atoi(argv[4]);
	data.number_of_meals.is_set = (argc == 6);
	data.number_of_meals.value = INIT_WITH_ZERO;
	if (data.number_of_meals.is_set)
		data.number_of_meals.value = ft_atoi(argv[5]);
	return data;
}
