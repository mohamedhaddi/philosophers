/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/22 15:49:01 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/22 16:55:33 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/philo.h"

int	ft_atoi(const char *str)
{
	int				j;
	unsigned long	res;
	int				temp;

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
