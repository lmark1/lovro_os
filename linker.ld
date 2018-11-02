ENTRY(loader)
OUTPUT_FORMAT(elf32-i386)
OUTPUT_ARCH(i386:i386)

/*
    Collecting sections from different .o files.
*/

SECTIONS
{
    . = 0x0100000;
    
    /*
        Code and read only variables.
    */
    .text : 
    {
        *(.multiboot)
        *(.text*)
        *(.rodata)
    }
    
    /*
        Initialized variables and constructor initialization list.
    */
    .data :
    {
        start_ctors = .;
        KEEP(*(.init_array));
        KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)));
        end_ctors = .;
        
        *(.data)
    }
       
    /*
        Statically allocated variables that are not 
        explicitly initialied to any value. e.g. Stack
    */
    .bss : 
    {
        *(.bss) 
    }
    
    /*
        Comments can be removed,
    */
    /DISCARD/ :
    {
        *(.fini_array)
        *(.comment)
    }
}