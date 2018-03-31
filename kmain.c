#include "multiboot.h"

#include "common.h"
#include "framebuffer.h"
#include "interrupts.h"
#include "io.h"
#include "pic.h"
#include "pci.h"
#include "segmentation.h"
#include "serial.h"
#include "utils.h"

#define KERNEL_VIRTUAL_BASE 0xC0000000
#define VIRTUAL_ADDR(x) (x + KERNEL_VIRTUAL_BASE)

struct kernel_mem_info {
    unsigned int kernel_virtual_start;
    unsigned int kernel_virtual_end;
    unsigned int kernel_physical_start;
    unsigned int kernel_physical_end;
} __attribute__((packed));

void log_mmap(multiboot_uint32_t addr, multiboot_uint32_t length) {
    /* This scheme of checking the 'size' field is apparently so
       that the multiboot spec can add extra fields in the future
       without breaking things.
       (Probably not likely to actually happen for multiboot 1...) */
    multiboot_memory_map_t *entry = (multiboot_memory_map_t*)addr;
    while ((multiboot_uint32_t)entry < (addr + length)) {
        log("mmap entry at ");
        log_hex_n(entry->addr >> 32);
        log_hex_n(entry->addr & 0xffffffff);
        log("size: ");
        log_hex_n(entry->len >> 32);
        log_hex_n(entry->len & 0xffffffff);
        log("type: ");
        switch(entry->type) {
        case MULTIBOOT_MEMORY_AVAILABLE:
            log_n("AVAILABLE");
            break;
        case MULTIBOOT_MEMORY_RESERVED:
            log_n("RESERVED");
            break;
        default:
            log_n("UNKNOWN");
        }
        multiboot_uint32_t next_addr = (multiboot_uint32_t)entry + entry->size + sizeof(entry->size);
        entry = (multiboot_memory_map_t*)next_addr;
    }
}

void kmain(unsigned int ebx, struct kernel_mem_info kmi, unsigned int * page_directory) {

    serial_init();
    UNUSED(kmi);
    multiboot_info_t *mbinfo = (multiboot_info_t *) VIRTUAL_ADDR(ebx);

    if (!(MULTIBOOT_INFO_MODS & mbinfo->flags)) {
        log("No modules loaded");
        return;
    }
    if (!((MULTIBOOT_INFO_MEMORY | MULTIBOOT_INFO_MEM_MAP) & mbinfo->flags)) {
        log("Memory info unavailable");
        return;
    }
    if (mbinfo->mods_count != 1) {
        log("Unexpected number of modules loaded");
        return;
    }

    /* log_mmap(VIRTUAL_ADDR(mbinfo->mmap_addr), mbinfo->mmap_length); */
    /* log_n("Framebuffer:"); */
    /* log_hex_n(mbinfo->framebuffer_addr); */
    log_pci();

    // Map framebuffer to next 4mb page after the page the kernel is in
    page_directory[(0xC0000000 >> 22) + 1] = mbinfo->framebuffer_addr | 0x83;
    unsigned int * fb = (unsigned int *)0xC0400000;
    for (int i = 0; i < 100000; i++) {
        fb[i] = 0xffffffff;
    }

    multiboot_module_t *mod = (multiboot_module_t *) VIRTUAL_ADDR(mbinfo->mods_addr);
    void(*start_program)(void) = (void(*)(void)) VIRTUAL_ADDR(mod->mod_start);

    UNUSED(start_program);


    init_segmentation();
    init_interrupts();

    /* Framebuffer */
    char * message = "Hello, World!!!";
    for (int i = 0;; i++) {
        if (message[i] == 0) {
            break;
        }
        fb_write(message[i]);
    }
}
