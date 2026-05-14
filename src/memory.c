/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Arena memory read/write helpers
*/

#include "my.h"

int read_mem_int(vm_t *vm, int addr, int size)
{
    int value = 0;
    int norm;

    for (int i = 0; i < size; i++) {
        norm = ((addr + i) % MEM_SIZE + MEM_SIZE) % MEM_SIZE;
        value = (value << 8) | vm->arena[norm];
    }
    if (size == 2 && (value & 0x8000))
        value |= (int)0xFFFF0000;
    return value;
}

void write_mem_int(vm_t *vm, int addr, int value, int size)
{
    int norm;

    for (int i = size - 1; i >= 0; i--) {
        norm = ((addr + i) % MEM_SIZE + MEM_SIZE) % MEM_SIZE;
        vm->arena[norm] = (uint8_t)(value & 0xFF);
        value >>= 8;
    }
}
