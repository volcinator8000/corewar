/*
** EPITECH PROJECT, 2025
** minishell1
** File description:
** my_atoi.c
*/

#include <limits.h>
#include "my.h"

int is_digit(char c)
{
    return (c >= '0' && c <= '9');
}

int skip_whitespace(const char *str)
{
    int i = 0;

    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' ||
        str[i] == '\r' || str[i] == '\f' || str[i] == '\v') {
        i++;
    }
    return i;
}

int get_sign(const char *str, int *index)
{
    int sign = 1;

    if (str[*index] == '+' || str[*index] == '-') {
        if (str[*index] == '-')
            sign = -1;
        (*index)++;
    }
    return sign;
}

long build_number(const char *str, int index, int sign)
{
    long result = 0;

    while (is_digit(str[index])) {
        result = result * 10 + (str[index] - '0');
        if (sign == 1 && result > INT_MAX)
            return INT_MAX;
        if (sign == -1 && -result < INT_MIN)
            return INT_MIN;
        index++;
    }
    return result * sign;
}

int my_atoi(const char *str)
{
    int index;
    int sign;

    index = skip_whitespace(str);
    sign = get_sign(str, &index);
    return (int)build_number(str, index, sign);
}