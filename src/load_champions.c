/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Load champion .cor files into the arena
*/

#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include "my.h"

static int swap_endian(int val)
{
    return ((val >> 24) & 0x000000FF) | ((val >> 8) & 0x0000FF00) |
        ((val << 8) & 0x00FF0000) | ((val << 24) & 0xFF000000);
}

static int read_header(int fd, header_t *hdr, char *path)
{
    int ret = read(fd, hdr, sizeof(header_t));

    if (ret != (int)sizeof(header_t)) {
        display_error("Error: cannot read header of ");
        display_error(path);
        display_error("\n");
        return ERR_CODE;
    }
    hdr->magic = swap_endian(hdr->magic);
    hdr->prog_size = swap_endian(hdr->prog_size);
    if (hdr->magic != COREWAR_EXEC_MAGIC) {
        display_error("Error: invalid magic in ");
        display_error(path);
        display_error("\n");
        return ERR_CODE;
    }
    return SUCCESS;
}

static void assign_load_addresses(vm_t *vm)
{
    int spacing = MEM_SIZE / vm->nb_players;

    for (int i = 0; i < vm->nb_players; i++) {
        if (vm->players[i].load_addr == -1)
            vm->players[i].load_addr = (spacing * i) % MEM_SIZE;
    }
}

static int read_champ_body(int fd, header_t *hdr, uint8_t *buf, char *name)
{
    int ret = read(fd, buf, hdr->prog_size);

    if (ret != hdr->prog_size) {
        display_error("Error: cannot read bytecode of ");
        display_error(name);
        display_error("\n");
        return ERR_CODE;
    }
    return SUCCESS;
}

static int load_one_champion(vm_t *vm, int idx)
{
    header_t h;
    int fd = open(vm->players[idx].name, O_RDONLY);
    int addr = vm->players[idx].load_addr;
    uint8_t b[MEM_SIZE];

    if (fd < 0 || read_header(fd, &h, vm->players[idx].name) != SUCCESS
        || read_champ_body(fd, &h, b, vm->players[idx].name) != SUCCESS) {
        if (fd >= 0)
            close(fd);
        return ERR_CODE;
    }
    close(fd);
    for (int i = 0; i < h.prog_size; i++)
        vm->arena[(addr + i) % MEM_SIZE] = b[i];
    my_strncpy(vm->players[idx].name, h.prog_name, PROG_NAME_LENGTH);
    my_strncpy(vm->players[idx].comment, h.comment, COMMENT_LENGTH);
    vm->players[idx].prog_size = h.prog_size;
    return SUCCESS;
}

static void create_initial_processes(vm_t *vm)
{
    process_t *proc;

    for (int i = 0; i < vm->nb_players; i++) {
        proc = create_process(vm->players[i].load_addr,
            vm->players[i].number, vm->proc_id_counter, vm);
        proc->registers[1] = -vm->players[i].number;
        vm->proc_id_counter++;
        add_process(vm, proc);
    }
}

int load_champions(vm_t *vm)
{
    assign_load_addresses(vm);
    for (int i = 0; i < vm->nb_players; i++) {
        if (load_one_champion(vm, i) != SUCCESS)
            return ERR_CODE;
    }
    create_initial_processes(vm);
    return SUCCESS;
}
