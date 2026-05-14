/*
** EPITECH PROJECT, 2026
** corewar
** File description:
** Load champion .cor files into the arena
*/

#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "my.h"

static int read_header(int fd, header_t *hdr, char *path)
{
    int ret = read(fd, hdr, sizeof(header_t));

    if (ret != (int)sizeof(header_t)) {
        display_error("Error: cannot read header of ");
        display_error(path);
        display_error("\n");
        return ERR_CODE;
    }
    hdr->magic = ntohl(hdr->magic);
    hdr->prog_size = ntohl(hdr->prog_size);
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
    int i = 0;

    while (i < vm->nb_players) {
        if (vm->players[i].load_addr == -1)
            vm->players[i].load_addr = (spacing * i) % MEM_SIZE;
        i++;
    }
}

static int load_one_champion(vm_t *vm, int idx)
{
    header_t h;
    int fd;
    int addr;
    uint8_t b[MEM_SIZE];
    int ret;
    int i;

    fd = open(vm->players[idx].name, O_RDONLY);
    if (fd < 0) {
        display_error("Error: cannot open ");
        display_error(vm->players[idx].name);
        display_error("\n");
        return ERR_CODE;
    }
    if (read_header(fd, &h, vm->players[idx].name) != SUCCESS) {
        close(fd);
        return ERR_CODE;
    }
    ret = read(fd, b, h.prog_size);
    close(fd);
    if (ret != h.prog_size) {
        display_error("Error: cannot read bytecode\n");
        return ERR_CODE;
    }
    addr = vm->players[idx].load_addr;
    i = 0;
    while (i < h.prog_size) {
        vm->arena[(addr + i) % MEM_SIZE] = b[i];
        i++;
    }
    my_strncpy(vm->players[idx].name, h.prog_name, PROG_NAME_LENGTH);
    my_strncpy(vm->players[idx].comment, h.comment, COMMENT_LENGTH);
    vm->players[idx].prog_size = h.prog_size;
    return SUCCESS;
}

static void create_initial_processes(vm_t *vm)
{
    process_t *proc;
    int i = 0;

    while (i < vm->nb_players) {
        proc = create_process(vm->players[i].load_addr,
            vm->players[i].number, vm->proc_id_counter, vm);
        proc->registers[1] = -vm->players[i].number;
        vm->proc_id_counter++;
        add_process(vm, proc);
        i++;
    }
}

int load_champions(vm_t *vm)
{
    int i = 0;

    assign_load_addresses(vm);
    while (i < vm->nb_players) {
        if (load_one_champion(vm, i) != SUCCESS)
            return ERR_CODE;
        i++;
    }
    create_initial_processes(vm);
    return SUCCESS;
}
