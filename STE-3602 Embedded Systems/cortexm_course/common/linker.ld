ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 256K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

/* The stack starts at the top of RAM and grows downward (full descending) */
_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* Vector table first, so it ends up at address 0x0 */
    .isr_vector :
    {
        KEEP(*(.isr_vector))
    } > FLASH

    /* Program code and constants live in FLASH */
    .text :
    {
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* Where the initial values of .data are stored in FLASH */
    _sidata = LOADADDR(.data);

    /* .data lives in RAM, but its initial values are stored in FLASH (AT > FLASH) */
    .data :
    {
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    /* .bss lives in RAM and is set to zero by Reset_Handler */
    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM
}
