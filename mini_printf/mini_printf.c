/*
** EPITECH PROJECT, 2025
** miniprintf
** File description:
** mini_printf.c
*/

#include "../include/my.h"

int nb_len(int nb)
{
    int count = 1;

    while (nb / 10 > 0) {
        count++;
        nb = nb / 10;
    }
    return count;
}

int check_flag(const char *format, int i, va_list args)
{
    char *str;
    int nb;

    if (format[i + 1] == 'd' || format[i + 1] == 'i') {
        nb = va_arg(args, int);
        my_put_nbr(nb);
        return nb_len(nb);
    } else if (format[i + 1] == 's') {
        str = va_arg(args, char *);
        my_putstr(str);
        return my_strlen(str);
    }
    if (format[i + 1] == 'c') {
        my_putchar(va_arg(args, int));
        return 1;
    } else if (format[i + 1] == '%') {
        my_putchar('%');
        return 1;
    }
    return -2147483648;
}

int mini_printf(const char *format, ...)
{
    va_list args;
    int i;
    int return_value = 0;

    va_start(args, format);
    for (i = 0; format[i] != '\0' && return_value >= 0; i++) {
        if (format[i] == '%') {
            return_value += check_flag(format, i, args);
            i++;
        } else {
            my_putchar(format[i]);
            return_value++;
        }
    }
    va_end(args);
    return return_value;
}
