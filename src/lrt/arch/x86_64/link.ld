OUTPUT_ARCH(i386:x86-64)
OUTPUT_FORMAT(elf64-x86-64)
ENTRY(_start)

mb_magic = 0x1badb002;
mb_flags = 0x3;
mb_checksum = -(mb_magic + mb_flags);

SECTIONS
{
        . = 0x00100000;
        kstart = .;

        .init : ALIGN(16)
        {
                /* the mb_header has to be in the first 8k
                * of the file so I put it here.
                */
                LONG(mb_magic)
                LONG(mb_flags)
                LONG(mb_checksum)

                *(.init.text);
                *(.init.smp);
                *(.init.data);
        }

        .text ALIGN (16) :
        {
                *(.text .text.*);
                *(.gnu.linkonce.t*);
        }

        .rodata ALIGN (4K) :
        {
                *(.rodata .rodata.*)
                *(.gnu.linkonce.r*)
        }

        .eh_frame_hdr ALIGN(16) : {
                *(.eh_frame_hdr)
        }

        .eh_frame ALIGN(4) : ONLY_IF_RO {
                __eh_frame_start = .;
                *(.eh_frame)
                QUAD(0);
        }

        .gcc_except_table : ONLY_IF_RO {
                *(.gcc_except_table .gcc_except_table.*)
        }

        .ctors ALIGN(16) : {
               start_ctors = .;
               KEEP(*(SORT(.ctors.*)))
               KEEP(*(.ctors))
               end_ctors = .;
        }

        .dtors ALIGN(16) : {
                KEEP(*(SORT(.dtors.*)))
                KEEP(*(.dtors))
        }

        .note : ALIGN(16) {
              *(.note.gnu.build-id)
        }

        .eh_frame ALIGN(4) : ONLY_IF_RW {
              __eh_frame_start = .;
              *(.eh_frame)
              QUAD(0);
        }

        .gcc_except_table : ONLY_IF_RW {
        *(.gcc_except_table .gcc_except_table.*)
        }

        .data ALIGN (4K) :
        {
                *(.data .data.*)
                *(.gnu.linkonce.d*)
        }

        .bss :
        {
                sbss = .;
                *(.bss .bss.*)
                *(COMMON)
                *(.gnu.linkonce.b*)
                ebss = .;
        }

        static_ebb_ids :
        {
                static_ebb_ids_start = .;
                *(static_ebb_ids)
                static_ebb_ids_end = .;
        }

        /DISCARD/ :
        {
                *(.comment);
        }
        kend = ALIGN(4096);
}
