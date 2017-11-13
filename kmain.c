#include "io.h"
#include "pic.h"

#define UNUSED(x) (void)(x)

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

/* Some colors */
#define FB_BLACK 0
#define FB_WHITE 15

/* The I/O ports */

/* All the I/O ports are calculated relative to the data port. This is because
 * all serial ports (COM1, COM2, COM3, COM4) have their ports in the same
 * order, but they start at different values.
 */

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* The I/O port commands */

/* SERIAL_LINE_ENABLE_DLAB:
 * Tells the serial port to expect first the highest 8 bits on the data port,
 * then the lowest 8 bits will follow
 */
#define SERIAL_LINE_ENABLE_DLAB         0x80

/** serial_configure_baud_rate:
 *  Sets the speed of the data being sent. The default speed of a serial
 *  port is 115200 bits/s. The argument is a divisor of that number, hence
 *  the resulting speed becomes (115200 / divisor) bits/s.
 *
 *  @param com      The COM port to configure
 *  @param divisor  The divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),
         SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com),
         (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com),
         divisor & 0x00FF);
}

/** serial_configure_line:
 *  Configures the line of the given serial port. The port is set to have a
 *  data length of 8 bits, no parity bits, one stop bit and break control
 *  disabled.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_line(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
     */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

void serial_configure_buffer(unsigned short com)
{
    outb(SERIAL_FIFO_COMMAND_PORT(com), 0xC7);
}

void serial_configure_modem(unsigned short com)
{
    outb(SERIAL_MODEM_COMMAND_PORT(com), 0x03);
}


/** serial_is_transmit_fifo_empty:
 *  Checks whether the transmit FIFO queue is empty or not for the given COM
 *  port.
 *
 *  @param  com The COM port
 *  @return 0 if the transmit FIFO queue is not empty
 *          1 if the transmit FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}

/** fb_move_cursor:
 *  Moves the cursor of the framebuffer to the given position
 *
 *  @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

char * fb = (char *) 0x000B8000;

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
    fb[i * 2] = c;
    fb[i * 2 + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

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

void hello() {

    /* Serial Logging */
    serial_configure_baud_rate(SERIAL_COM1_BASE, 2);
    serial_configure_line(SERIAL_COM1_BASE);
    serial_configure_buffer(SERIAL_COM1_BASE);
    serial_configure_modem(SERIAL_COM1_BASE);

    while (!serial_is_transmit_fifo_empty(SERIAL_COM1_BASE));
    /* char * serial_message = "Hey there!"; */
    /* for (int i = 0; i < 10; i++) { */
    /*     outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), serial_message[i]); */
    /* } */
    /* outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), 10); */


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

    /* /\* Framebuffer *\/ */
    /* char * message = "Hello, World!!!"; */
    /* for (int i = 0; i < 15; i++) { */
    /*     fb_write_cell(i, message[i], FB_BLACK, FB_WHITE); */
    /* } */
    /* fb_move_cursor(15); */
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
        UNUSED(key);
        char * serial_message = "Key Pressed!";
        for (int i = 0; i < 12; i++) {
            outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), serial_message[i]);
        }
        outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE), 10);
        /* ACK */
        outb( 0x20, 0x20 ); /* master PIC */
    }
}
