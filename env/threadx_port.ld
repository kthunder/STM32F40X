MEMORY
{
	RAM		(xrw)	: ORIGIN = 0x20000000,	LENGTH = 64K
	FLASH	(rx)	: ORIGIN = 0x8000000,	LENGTH = 256K
}

__STACKSIZE__ = 1024;
__STACKSIZE_PROCESS__ = 0;
__HEAPSIZE__ = 128;

SECTIONS
{
	.text :
	{
		KEEP(*(.vectors .vectors.*))
		*(.text*)
		*(.rodata*)
	} > FLASH

	.reserved :
	{
		KEEP(*(.init))
		KEEP(*(.fini))

		*(.ARM.extab* .gnu.linkonce.armextab.*)
		__exidx_start = .;

		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
		__exidx_end = .;

		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(SORT(.preinit_array.*)))
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);

		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);

		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);
	} > FLASH

	.data :
	{
		*(.data*)
		*(.data)
	} > RAM AT > FLASH

	__INIT_DATA_ROM_START = LOADADDR(.data);
	__INIT_DATA_RAM_START = ORIGIN(RAM);
	__INIT_DATA_RAM_END   = ORIGIN(RAM) + SIZEOF(.data);

	.bss (NOLOAD) :
	{
		*(.bss*)
		*(.bss)
	} > RAM AT > RAM
	__BSS_DATA_RAM_START = LOADADDR(.bss);
	__BSS_DATA_RAM_END   = LOADADDR(.bss) + SIZEOF(.bss);
	
	.heap (COPY):
	{
		__heap_start__ = .;
		*(.heap)
		. += __HEAPSIZE__;
		__heap_end__ = .;
	} > RAM AT > RAM

	.stack (NOLOAD) :
	{
		__stack_start__ = .;
		*(.stack)
		. += __STACKSIZE__;
		__stack_end__ = .;
	} > RAM AT > RAM

	__RAM_segment_used_end__ = .;
}
