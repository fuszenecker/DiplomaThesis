/* ---------------------------------------------------------------------------
 * Linker script file for ARM Cortex-M3 microcontrollers
 * ------------------------------------------------------------------------ */

MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x20000
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x2000
}

/* Section Definitions */
SECTIONS
{
    /* Code and constant data */
    .text :
    {
	    _pointers = .;

        /* Initial value of the stack pointer after RESET */
	    *(.stack_pointer)

        /* Pointers to Interrupt (including RESET) service routines */
	    *(.vectors)
	    *(.isr_vector)
    
        /* Code sections */
	    _text = .;
	    *(.text .text.*)

        /* Read-only data */
	    *(.rodata .rodata*)
	    _etext = .;
	    _sidata = .;
    } > FLASH
   
    /* Initialized data (read-write) */
    .data : AT (_etext)
    {
	    _data = .;
	    _sdata = .;
	    *(.data .data.*)
	    _edata = . ;
    } > SRAM
 
    /* Uninitialized data (heap memory + stack) */
    .bss (NOLOAD) :
    {
	    _bss = . ;
	    _sbss = .;
	    *(.bss .bss.*)
	    _ebss = . ;
    } > SRAM
  
    . = ALIGN(4);
    _end = . ;
    _estack = . ;

    /* Stack will be at the end of the RAM area */
}
