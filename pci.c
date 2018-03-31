#include "common.h"
#include "io.h"
#include "utils.h"

#define PCI_ADDR_PORT 0x0CF8
#define PCI_DATA_PORT 0x0CFC
#define PCI_ADDR_ENABLE 1 << 31
#define PCI_BUS_OFFSET 16
#define PCI_DEVICE_OFFSET 11
#define PCI_FUNCTION_OFFSET 8
#define PCI_MAX_DEVICES 32
#define PCI_MAX_FUNCTIONS 8
#define PCI_CLASS_CODES_REGISTER 0xA
#define PCI_CLASS_CODES_SIZE 2 // Combined size of class, subclass
#define PCI_USB_CLASS_CODES 0x0C03 // Combined USB class, subclass
#define PCI_VENDOR_ID_REGISTER 0
#define PCI_VENDOR_ID_SIZE 2
#define PCI_DEVICE_ID_REGISTER 2
#define PCI_DEVICE_ID_SIZE 2

unsigned int read_pci(unsigned char bus, unsigned char dev, unsigned char func, unsigned char reg_num, unsigned char size) {
    unsigned int addr_reg =
        PCI_ADDR_ENABLE |
        (bus << PCI_BUS_OFFSET) |
        (dev << PCI_DEVICE_OFFSET) |
        (func << PCI_FUNCTION_OFFSET) |
        // Bits 0:1 must be 0. The spec doesn't seem clear about this,
        // but the Register Number should be rounded down to the nearest
        // dword rather than shifting to the position 7:2.
        (reg_num & 0xFC);
    outd(PCI_ADDR_PORT, addr_reg);
    unsigned char offset = reg_num & 0x3;
    unsigned int result = ind(PCI_DATA_PORT) >> (offset * 8);
    result &= 0xFFFFFFFF >> ((4 - size) * 8);
    return result;
}

void log_pci() {
    for (int dev = 0; dev < PCI_MAX_DEVICES; dev++) {
        for (int func = 0; func < PCI_MAX_FUNCTIONS; func++) {
            // Only supporting a single bus for now
            if (read_pci(0, dev, func, PCI_CLASS_CODES_REGISTER, PCI_CLASS_CODES_SIZE) == PCI_USB_CLASS_CODES) {
                log_n("Detected a USB controller:");
                log("  Vendor ID: ");
                log_hex_n(read_pci(0, dev, func, PCI_VENDOR_ID_REGISTER, PCI_VENDOR_ID_SIZE));
                log("  Device ID: ");
                log_hex_n(read_pci(0, dev, func, PCI_DEVICE_ID_REGISTER, PCI_DEVICE_ID_SIZE));
            }
        }
    }
}
