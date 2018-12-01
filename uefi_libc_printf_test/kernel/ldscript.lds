OUTPUT_ARCH(arm)
ENTRY(_start)
SECTIONS
{
	. = 0x40100000;
	
	.start : { start.o(.text) }

	.text : { *(.text*) }
	. = ALIGN(4);

	__rodata_start = .;
	.rodata : { *(.rodata*) }
	. = ALIGN(4);
	__rodata_end = .;

	__data_start = . ;
	.data : { *(.data*) }
	. = ALIGN(4);
	__data_end = . ;

	__bss_start = . ;
	.bss : { *(.bss*) }
	. = ALIGN(4);
	__bss_end = . ;

	. = ALIGN(4096);
	__free_start = .;
    

    /* stack space for EL0 */
	. = . + 8M; 
	__el0_stack_init = .;

    /* stack space for EL1 */
	. = . + 0x00800000;
	__el1_stack_init = .;
}