/*
** EPITECH PROJECT, 2025
** G-CPE-100-LYN-1-1-cpoolday03-14
** File description:
** my_put_nbr.c
*/

#include "../include/my.h"

int my_put_nbr(int nb)
{
    if (nb == -2147483648) {
        write(1, "-2147483648", 11);
        return 0;
    }
    if (nb < 0) {
        my_putchar('-');
        nb *= -1;
    }
    if (nb / 10 >= 1)
        my_put_nbr(nb / 10);
    my_putchar(nb % 10 + 48);
    return 0;
}
