/* https://www-s.acm.illinois.edu/sigops/2007/roll_your_own/i386/irq.html */
#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT 0xA1
#define SANE_ICW1 0x11
#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

void sane_interrupts();
