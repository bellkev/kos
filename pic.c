#include "io.h"

/* https://www-s.acm.illinois.edu/sigops/2007/roll_your_own/i386/irq.html */
#define PIC1_COMMAND_PORT 0x20
#define PIC1_DATA_PORT 0x21
#define PIC2_COMMAND_PORT 0xA0
#define PIC2_DATA_PORT 0xA1
#define SANE_ICW1 0x11
#define PIC1_OFFSET 0x20
#define PIC2_OFFSET 0x28

void sane_interrupts() {
    /* ICW1 */
    outb(PIC1_COMMAND_PORT, SANE_ICW1);
    outb(PIC2_COMMAND_PORT, SANE_ICW1);

    /* ICW2 */
    outb(PIC1_DATA_PORT, PIC1_OFFSET); /* Master offset of 0x20 in the IDT */
    outb(PIC2_DATA_PORT, PIC2_OFFSET); /* Master offset of 0x28 in the IDT */

    /* ICW3 */
    outb(PIC1_DATA_PORT, 0x04); /* Slaves attached to IR line 2 */
    outb(PIC1_DATA_PORT, 0x02); /* This slave in IR line 2 of master */

    /* ICW4 */
    outb(PIC1_DATA_PORT, 0x01); /* Set as master */
    outb(PIC2_DATA_PORT, 0x01); /* Set as slave */

    /* Mask everything but the keyboard */
    outb(PIC1_DATA_PORT, 0xfd); /* master PIC */
    outb(PIC2_DATA_PORT, 0xff); /* slave PIC */
}
