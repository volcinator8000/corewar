/*
** EPITECH PROJECT, 2026
** epitech
** File description:
** my.h
*/

#ifndef VM_H_
#define VM_H_

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <unistd.h>
#include "op.h"

#define MAX_PLAYERS 4
#define ERR_CODE 84
#define SUCCESS 0

typedef struct process_s {
    int id;
    int player_id;
    int pc;
    int registers[REG_NUMBER + 1];
    int carry;
    int live_count;
    int wait_cycles;
    int current_op;
    struct process_s *next;
} process_t;

typedef struct player_s {
    int number;
    int load_addr;
    char name[PROG_NAME_LENGTH + 1];
    char comment[COMMENT_LENGTH + 1];
    int prog_size;
    int last_live;
} player_t;

typedef struct vm_s {
    uint8_t arena[MEM_SIZE];
    player_t players[MAX_PLAYERS];
    int nb_players;
    process_t *processes;
    int nb_processes;
    int cycle;
    int cycle_to_die;
    int last_live;
    int live_count;
    int checks;
    int dump_cycle;
    int proc_id_counter;
} vm_t;

typedef void (*op_func_t)(vm_t *vm, process_t *proc);

int parse_args(int ac, char **av, vm_t *vm);
int load_champions(vm_t *vm);
void run_vm(vm_t *vm);
void dump_state(vm_t *vm);

process_t *create_process(int pc, int player_id, int id, vm_t *vm);
void add_process(vm_t *vm, process_t *proc);
void remove_dead_processes(vm_t *vm);

int read_mem_int(vm_t *vm, int addr, int size);
void write_mem_int(vm_t *vm, int addr, int value, int size);

void execute_process(vm_t *vm, process_t *proc);

int get_arg_type(int t);
int decode_arg_mod(vm_t *vm, process_t *proc, int arg_type, int offset);
int decode_arg_at(vm_t *vm, process_t *proc, int arg_type, int offset);

void op_live(vm_t *vm, process_t *proc);
void op_ld(vm_t *vm, process_t *proc);
void op_st(vm_t *vm, process_t *proc);
void op_add(vm_t *vm, process_t *proc);
void op_sub(vm_t *vm, process_t *proc);
void op_and(vm_t *vm, process_t *proc);
void op_or(vm_t *vm, process_t *proc);
void op_xor(vm_t *vm, process_t *proc);
void op_zjmp(vm_t *vm, process_t *proc);
void op_ldi(vm_t *vm, process_t *proc);
void op_sti(vm_t *vm, process_t *proc);
void op_fork(vm_t *vm, process_t *proc);
void op_lld(vm_t *vm, process_t *proc);
void op_lldi(vm_t *vm, process_t *proc);
void op_lfork(vm_t *vm, process_t *proc);
void op_print(vm_t *vm, process_t *proc);

void display_error(char *error);
int my_strcmp(const char *s1, const char *s2);
char *my_strncpy(char *dest, char const *src, int n);
int mini_printf(const char *format, ...);
int my_put_nbr(int nb);
void my_putchar(char c);
int my_putstr(char const *str);
int my_strlen(char const *str);
char *hexa_converter(unsigned int value, int n, char *buf);
int my_atoi(const char *str);

#endif
