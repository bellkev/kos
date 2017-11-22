global load_idt
global common_interrupt_handler
extern interrupt_handler

common_interrupt_handler:               ; the common parts of the generic interrupt handler
  ; save the registers in a CpuState struct, see interrupts.h
  push    esp
  push    ebp
  push    edi
  push    esi
  push    edx
  push    ecx
  push    ebx
  push    eax

  ; call the C function
  call    interrupt_handler

  ; restore the registers
  pop     eax
  pop     ebx
  pop     ecx
  pop     edx
  pop     esi
  pop     edi
  pop     ebp
  ; Don't need to pop esp, that's done below.

  ; pop esp, interrupt_number, and error_code
  add     esp, 12

  ; return to the code that got interrupted
  iret

; load_idt - Loads the interrupt descriptor table (IDT).
; stack: [esp + 4] the address of the first entry in the IDT
;        [esp    ] the return address
load_idt:
        mov     eax, [esp+4]    ; load the address of the IDT into register eax
        lidt    [eax]           ; load the IDT
        ret                     ; return to the calling function

global clear_interrupt
clear_interrupt:
  cli ; disable interrupts
  ret

global set_interrupt
set_interrupt:
  sti ; enable interrupts
  ret

global load_gdt

; lgdt - load gdt, assumes the following gdt layout:
;     0: null segment descriptor
;     1: code segment descriptor
;     2: data segment descriptor
;   [esp+4] location of the gdt struct
load_gdt:
  mov eax, [esp + 4]
  lgdt [eax]
  mov ax, 0x0010
  mov ss, ax
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  ; other data segments?
  ; far jump to update cs to GDT:0x08
  jmp 0x08:flush_cs
  flush_cs:
  ret
