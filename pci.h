struct pci_config {
    unsigned short vendor_id;
    unsigned short device_id;
};

void log_pci(unsigned char bus, unsigned char dev, unsigned char func);
