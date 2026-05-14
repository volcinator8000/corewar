/*
** EPITECH PROJECT, 2025
** cpoolday4
** File description:
** my_strlen.c
*/

int my_strlen(char const *str)
{
    int count = 0;
    int i = 0;

    while (str[i] != '\0') {
        count++;
        i++;
    }
    return count;
}
