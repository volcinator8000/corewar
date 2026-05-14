/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** into to hexa
*/

char *hexa_converter(unsigned int value, int n, char *buf)
{
    char hex[17] = "0123456789ABCDEF";
    int i = n - 1;

    buf[n] = '\0';
    while (i >= 0) {
        buf[i] = hex[value % 16];
        value /= 16;
        i--;
    }
    return buf;
}