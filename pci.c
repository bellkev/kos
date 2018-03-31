#include "io.h"
#include "utils.h"

#define PCI_ADDR_PORT 0x0CF8
#define PCI_DATA_PORT 0x0CFC
#define PCI_ADDR_ENABLE 1 << 31
#define PCI_BUS_OFFSET 16
#define PCI_DEVICE_OFFSET 11
#define PCI_FUNCTION_OFFSET 8

void log_pci(unsigned char bus, unsigned char dev, unsigned char func) {
    unsigned int val = 0;
    val |= PCI_ADDR_ENABLE;
    val |= bus << PCI_BUS_OFFSET;
    val |= dev << PCI_DEVICE_OFFSET;
    val |= func << PCI_FUNCTION_OFFSET;
    outd(PCI_ADDR_PORT, val);
    log_n("Vendor ID:");
    log_hex_n(0xFFFF & ind(PCI_DATA_PORT));
}
