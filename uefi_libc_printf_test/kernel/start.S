.global _start
.align 4

_start:
    // set el1 stack pointer
    ldr x4, =__el1_stack_init
    //msr SP_EL1, x4
    mov sp, x4
    // disable all interrupt(daif at bits 9..6)
    msr DAIFSet, #0x0f
    // jump to early_init
    bl early_init
fin:
    // disable all interrupt(daif at bits 9..6)
    msr DAIFSet, #0x0f
    wfi
    b fin