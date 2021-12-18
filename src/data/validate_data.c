/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_data.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 18:25:13 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/18 17:41:56 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

bool is_valid_data(t_data data)
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
