/*
** EPITECH PROJECT, 2025
** corewar
** File description:
** my_strcmp.c
*/

#include "my.h"

int my_strcmp(const char *s1, const char *s2)
{
    int i;

    for (i = 0; s1[i] == s2[i]; i++) {
        if (s1[i] == '\0')
            return 0;
    }
    return s1[i] - s2[i];
}
