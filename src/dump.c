/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** VM state dump for -dump flag
*/

#include "my.h"

static void print_player_header(vm_t *vm, process_t *proc)
{
    int i = 0;

    while (i < vm->nb_players) {
        if (vm->players[i].number == proc->player_id)
            mini_printf("%s (%d): alive\n",
                vm->players[i].name, proc->player_id);
        i++;
    }
}

static void print_registers(process_t *proc)
{
    int r = 1;
    char hexa[9];

    while (r <= REG_NUMBER) {
        mini_printf("r%d:%s\n", r,
            hexa_converter((unsigned int)proc->registers[r], 8, hexa));
        r++;
    }
    mini_printf("PC:%s\n", hexa_converter((unsigned int)proc->pc, 8, hexa));
    mini_printf("carry:%d\n", proc->carry);
}

static void dump_processes(vm_t *vm)
{
    process_t *cur = vm->processes;

    while (cur) {
        print_player_header(vm, cur);
        print_registers(cur);
        cur = cur->next;
    }
}

static void dump_memory_row(vm_t *vm, int i)
{
    int j = 0;
    char hexa_8[9];
    char hexa_2[3];

    mini_printf("%s:", hexa_converter(i, 8, hexa_8));
    while (j < 32 && i + j < MEM_SIZE) {
        mini_printf("%s",
            hexa_converter(vm->arena[i + j], 2, hexa_2));
        j++;
    }
    mini_printf("\n");
}

static void dump_memory(vm_t *vm)
{
    int i = 0;

    mini_printf("Memory:\n");
    mini_printf("000102030405060708090A0B0C0D0E0F"
        "101112131415161718191A1B1C1D1E1F\n");
    mini_printf("------------------------------------------"
        "--------------------------------------\n");
    while (i < MEM_SIZE) {
        dump_memory_row(vm, i);
        i += 32;
    }
}

void dump_state(vm_t *vm)
{
    mini_printf("Cycle:%d\n", vm->cycle);
    mini_printf("Registers:\n");
    dump_processes(vm);
    dump_memory(vm);
}
