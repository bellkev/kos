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
