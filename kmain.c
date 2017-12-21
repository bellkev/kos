#include "multiboot.h"

#include "common.h"
#include "framebuffer.h"
#include "interrupts.h"
#include "io.h"
#include "pic.h"
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

void kmain(unsigned int ebx, struct kernel_mem_info kmi, unsigned int * page_directory) {

    serial_init();
    log("EBX:");
    log_hex(ebx);
    UNUSED(kmi);
    multiboot_info_t *mbinfo = (multiboot_info_t *) VIRTUAL_ADDR(ebx);

    // Map framebuffer to next 4mb page after the page the kernel is in
    page_directory[(0xC0000000 >> 22) + 1] = mbinfo->framebuffer_addr | 0x83;
    unsigned int * fb = (unsigned int *)0xC0400000;
    for (int i = 0; i < 100000; i++) {
        fb[i] = 0xffffffff;
    }

    if (!(MULTIBOOT_INFO_MODS & mbinfo->flags)) {
        log("No modules loaded");
        return;
    }
    if (!(MULTIBOOT_INFO_MEMORY & mbinfo->flags)) {
        log("Memory info unavailable");
        return;
    }
    if (mbinfo->mods_count != 1) {
        log("Unexpected number of modules loaded");
        return;
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
    log("In the higher half!");
}
