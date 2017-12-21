#include "io.h"
#include "pic.h"

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
