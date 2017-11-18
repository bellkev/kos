#include "framebuffer.h"
#include "io.h"
#include "pic.h"
#include "serial.h"
#include "scancodes.h"
#include "utils.h"

#define UNUSED(x) (void)(x)

struct gdt_spec {
    unsigned short size;
    unsigned int address;
} __attribute__((packed));

void load_gdt(struct gdt_spec * gdt);

struct segment_descriptor {
    unsigned short limit;
    unsigned short base_15_0;
    unsigned char base_23_16;
    unsigned short flags;
    unsigned char base_31_24;
} __attribute__((packed));


struct idt_spec {
    unsigned short size;
    unsigned int address;
} __attribute__((packed));

void load_idt(struct idt_spec * idt);

struct cpu_state {
  unsigned int eax;
  unsigned int ebx;
  unsigned int ecx;
  unsigned int edx;
  unsigned int esi;
  unsigned int edi;
  unsigned int ebp;
  unsigned int esp;
} __attribute__((packed));

struct stack_state {
    unsigned int error_code;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
} __attribute__((packed));

struct idt_entry {
    unsigned short offset_low;
    unsigned short segment;
    unsigned short flags;
    unsigned short offset_high;
} __attribute__((packed));

void fill_idt_entry(struct idt_entry * entry, unsigned int fn) {
    entry->offset_low = fn & 0xFFFF;
    /* Segment Index|GDT(0)/LDT(1)|Privelege(0-3) */
    /* 0000000000001|0|00 */
    entry->segment = 0x0008;
    entry->flags = 0x8F00;
    entry->offset_high = fn >> 16 & 0xFFFF;
}

void foo_interrupt_handler();
void interrupt_handler_0();
void interrupt_handler_1();
void interrupt_handler_2();
void interrupt_handler_3();
void interrupt_handler_4();
void interrupt_handler_5();
void interrupt_handler_6();
void interrupt_handler_7();
void interrupt_handler_8();
void interrupt_handler_9();
void interrupt_handler_10();
void interrupt_handler_11();
void interrupt_handler_12();
void interrupt_handler_13();
void interrupt_handler_14();
void interrupt_handler_15();
void interrupt_handler_16();
void interrupt_handler_17();
void interrupt_handler_18();
void interrupt_handler_19();
void interrupt_handler_20();
void interrupt_handler_21();
void interrupt_handler_22();
void interrupt_handler_23();
void interrupt_handler_24();
void interrupt_handler_25();
void interrupt_handler_26();
void interrupt_handler_27();
void interrupt_handler_28();
void interrupt_handler_29();
void interrupt_handler_30();
void interrupt_handler_31();
void interrupt_handler_32();
void interrupt_handler_33();
void interrupt_handler_34();
void interrupt_handler_35();
void interrupt_handler_36();
void interrupt_handler_37();
void interrupt_handler_38();
void interrupt_handler_39();
void interrupt_handler_40();
void interrupt_handler_41();
void interrupt_handler_42();
void interrupt_handler_43();
void interrupt_handler_44();
void interrupt_handler_45();
void interrupt_handler_46();
void interrupt_handler_47();


void set_interrupt();
void clear_interrupt();

struct segment_descriptor gdt[5];

void init_segmentation() {
  // leave 0 (null) segment as is

  // segment 1 is for code
  gdt[1].base_31_24 = 0x00;
  // 0b1100 (4 byte granularity, 32-bit operations, no 64-bit operations)
  // 0b1111 (high nibble of segment limit)
  // 0b1001 (present in memory, privilege = 0, data/code segment
  // 0b1010 (execute/read, non-conforming, non-accessed)
  gdt[1].flags = 0xCF9A;
  gdt[1].base_23_16 = 0x00;
  gdt[1].base_15_0 = 0x0000;
  gdt[1].limit = 0xFFFF;

  // segment 2 is for data
  gdt[2].base_31_24 = 0x00;
  // 0b1100 (4 byte granularity, 32-bit operations, no 64-bit operations)
  // 0b1111 (high nibble of segment limit)
  // 0b1001 (present in memory, privilege = 0, data/code segment
  // 0b0010 (read/write, no-expand down [?], non-accessed)
  gdt[2].flags = 0xCF92;
  gdt[2].base_23_16 = 0x00;
  gdt[2].base_15_0 = 0x0000;
  gdt[2].limit = 0xFFFF;

  // segment 3 is for user code
  gdt[3].base_31_24 = 0x00;
  // 0b1100 (4 byte granularity, 32-bit operations, no 64-bit operations)
  // 0b1111 (high nibble of segment limit)
  // 0b1001 (present in memory, privilege = 3, data/code segment
  // 0b1010 (execute/read, non-conforming, non-accessed)
  gdt[3].flags = 0xCFFA;
  gdt[3].base_23_16 = 0x00;
  gdt[3].base_15_0 = 0x0000;
  gdt[3].limit = 0xFFFF;

  // segment 4 is for user data
  gdt[4].base_31_24 = 0x00;
  // 0b1100 (4 byte granularity, 32-bit operations, no 64-bit operations)
  // 0b1111 (high nibble of segment limit)
  // 0b1001 (present in memory, privilege = 3, data/code segment
  // 0b0010 (read/write, no-expand down [?], non-accessed)
  gdt[4].flags = 0xCFF2;
  gdt[4].base_23_16 = 0x00;
  gdt[4].base_15_0 = 0x0000;
  gdt[4].limit = 0xFFFF;

  struct gdt_spec gs;
  gs.address = (unsigned int)gdt;
  gs.size = sizeof(gdt);

  load_gdt(&gs);
}


struct idt_entry table[48];
int cursor = 0;

void hello() {

    serial_init();
    init_segmentation();

    /* Interrupts */
    clear_interrupt();
    sane_interrupts();
    fill_idt_entry(&table[0], (unsigned int)interrupt_handler_0);
    fill_idt_entry(&table[1], (unsigned int)interrupt_handler_1);
    fill_idt_entry(&table[2], (unsigned int)interrupt_handler_2);
    fill_idt_entry(&table[3], (unsigned int)interrupt_handler_3);
    fill_idt_entry(&table[4], (unsigned int)interrupt_handler_4);
    fill_idt_entry(&table[5], (unsigned int)interrupt_handler_5);
    fill_idt_entry(&table[6], (unsigned int)interrupt_handler_6);
    fill_idt_entry(&table[7], (unsigned int)interrupt_handler_7);
    fill_idt_entry(&table[8], (unsigned int)interrupt_handler_8);
    fill_idt_entry(&table[9], (unsigned int)interrupt_handler_9);
    fill_idt_entry(&table[10], (unsigned int)interrupt_handler_10);
    fill_idt_entry(&table[11], (unsigned int)interrupt_handler_11);
    fill_idt_entry(&table[12], (unsigned int)interrupt_handler_12);
    fill_idt_entry(&table[13], (unsigned int)interrupt_handler_13);
    fill_idt_entry(&table[14], (unsigned int)interrupt_handler_14);
    fill_idt_entry(&table[15], (unsigned int)interrupt_handler_15);
    fill_idt_entry(&table[16], (unsigned int)interrupt_handler_16);
    fill_idt_entry(&table[17], (unsigned int)interrupt_handler_17);
    fill_idt_entry(&table[18], (unsigned int)interrupt_handler_18);
    fill_idt_entry(&table[19], (unsigned int)interrupt_handler_19);
    fill_idt_entry(&table[20], (unsigned int)interrupt_handler_20);
    fill_idt_entry(&table[21], (unsigned int)interrupt_handler_21);
    fill_idt_entry(&table[22], (unsigned int)interrupt_handler_22);
    fill_idt_entry(&table[23], (unsigned int)interrupt_handler_23);
    fill_idt_entry(&table[24], (unsigned int)interrupt_handler_24);
    fill_idt_entry(&table[25], (unsigned int)interrupt_handler_25);
    fill_idt_entry(&table[26], (unsigned int)interrupt_handler_26);
    fill_idt_entry(&table[27], (unsigned int)interrupt_handler_27);
    fill_idt_entry(&table[28], (unsigned int)interrupt_handler_28);
    fill_idt_entry(&table[29], (unsigned int)interrupt_handler_29);
    fill_idt_entry(&table[30], (unsigned int)interrupt_handler_30);
    fill_idt_entry(&table[31], (unsigned int)interrupt_handler_31);
    fill_idt_entry(&table[32], (unsigned int)interrupt_handler_32);
    fill_idt_entry(&table[33], (unsigned int)interrupt_handler_33);
    fill_idt_entry(&table[34], (unsigned int)interrupt_handler_34);
    fill_idt_entry(&table[35], (unsigned int)interrupt_handler_35);
    fill_idt_entry(&table[36], (unsigned int)interrupt_handler_36);
    fill_idt_entry(&table[37], (unsigned int)interrupt_handler_37);
    fill_idt_entry(&table[38], (unsigned int)interrupt_handler_38);
    fill_idt_entry(&table[39], (unsigned int)interrupt_handler_39);
    fill_idt_entry(&table[40], (unsigned int)interrupt_handler_40);
    fill_idt_entry(&table[41], (unsigned int)interrupt_handler_41);
    fill_idt_entry(&table[42], (unsigned int)interrupt_handler_42);
    fill_idt_entry(&table[43], (unsigned int)interrupt_handler_43);
    fill_idt_entry(&table[44], (unsigned int)interrupt_handler_44);
    fill_idt_entry(&table[45], (unsigned int)interrupt_handler_45);
    fill_idt_entry(&table[46], (unsigned int)interrupt_handler_46);
    fill_idt_entry(&table[47], (unsigned int)interrupt_handler_47);


    struct idt_spec idt;
    idt.size = sizeof(table);
    idt.address = (unsigned int)table;
    load_idt(&idt);
    set_interrupt();

    /* Framebuffer */
    char * message = "Hello, World!!!";
    for (int i = 0; i < 15; i++) {
        fb_write(&cursor, message[i]);
    }
}

#define KBD_DATA_PORT   0x60

/** read_scan_code:
 *  Reads a scan code from the keyboard
 *
 *  @return The scan code (NOT an ASCII character!)
 */
unsigned char read_scan_code(void)
{
    return inb(KBD_DATA_PORT);
}

void interrupt_handler(struct cpu_state cpu, unsigned int interrupt, struct stack_state stack) {
    UNUSED(cpu);
    UNUSED(stack);
    /* UNUSED(interrupt); */

    /* Only listen to the keyboard */
    if (interrupt >= 0x20 && interrupt < 0x30) {
        unsigned char key = read_scan_code();
        log("Key scan code received:");
        log_hex(key);
        int ascii = code_to_ascii(key);
        if (ascii) {
            fb_write(&cursor, ascii);
        }
        /* ACK */
        outb( 0x20, 0x20 ); /* master PIC */
    }
}
