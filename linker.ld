ENTRY(_start)

SECTIONS
{
  /* Start sections at 1MB address, typical for kernel loading */
  . = 1M;

  /* Text segment */
  .text BLOCK(4K) : ALIGN(4K) 
  {
    /* Place multiboot earlier so bootloader will identify it */
    *(.multiboot)
    /* Place text section after multiboot header */
    *(.text)
  }

  /* Read-only data */
  .rodata BLOCK(4K) : ALIGN(4K)
  {
    *(.rodata)
  }

  /* Data segment */
  .data BLOCK(4K) : ALIGN(4K)
  {
    *(.data)
  }

  /* BSS and Stack */
  .bss BLOCK(4K) : ALIGN(4K)
  {
    *(COMMON)
    *(.bss)
  }

}
