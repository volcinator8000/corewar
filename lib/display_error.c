/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** error display
*/

#include "my.h"

void display_error(char *error)
{
    write(2, error, my_strlen(error));
    return;
}
