/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaddi <mhaddi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/12 18:19:11 by mhaddi            #+#    #+#             */
/*   Updated: 2021/12/12 18:27:03 by mhaddi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philo.h"

int ft_isdigit(int c)
{
    if (c >= 48 && c <= 57)
        return (1);
    return (0);
}

bool    is_integer(char *str)
{
    int i;

    i = 0;
    if (str[0] == '-' && str[1])
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (false);
        i++;
    }
    return (true);
}
void    init_index(int *i, bool is_negative)
{
    *i = 0;
    if (is_negative)
        *i = 1;
}

void    set_last_digit(char *last_digit, bool is_negative)
{
    *last_digit = '7';
    if (is_negative)
        *last_digit = '8';
}

bool    check_every_digit(int i, int len, char last_digit, char **strings)
{
    int     j;
    char    *str;
    char    *max_int;

    str = strings[0];
    max_int = strings[1];
    j = 0;
    while (i < len - 1)
    {
        if (str[i] > max_int[j])
            return (true);
        else if (str[i] < max_int[j])
            return (false);
        i++;
        j++;
    }
    if (str[i] > last_digit)
        return (true);
    return (false);
}

size_t  ft_strlen(const char *s)
{
    size_t  i;

    i = 0;
    while (s[i] != '\0')
        i++;
    return (i);
}

bool    is_over_int(char *str)
{
    char    *max_int;
    int     len;
    bool    is_negative;
    int     i;
    char    last_digit;

    max_int = "2147483647";
    len = ft_strlen(str);
    is_negative = str[0] == '-';
    init_index(&i, is_negative);
    set_last_digit(&last_digit, is_negative);
    if ((is_negative && len > 11) || (!is_negative && len > 10))
        return (true);
    if ((is_negative && len < 11) || (!is_negative && len < 10))
        return (false);
    return (check_every_digit(i, len, last_digit, (char *[2]){str, max_int}));
}

int are_valid_numbers(char **argv, int argc)
{
    int i;

    i = 1;
    while (i < argc)
    {
	    if (is_over_int(argv[i]) || !is_integer(argv[i]))
		    return (false);
        i++;
    }
    return (true);
}

