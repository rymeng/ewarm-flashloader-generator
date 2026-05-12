        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .text:CODE:ROOT(2)
        PUBLIC  FlashInitEntry
        PUBLIC  FlashSignoffEntry
        PUBLIC  FlashWriteEntry
        PUBLIC  FlashEraseWriteEntry
        PUBLIC  FlashChecksumEntry
        EXTERN  mem_region_init
        EXTERN  mem_region_deinit
        EXTERN  mem_region_program
        EXTERN  mem_region_erase_program
        EXTERN  mem_region_verify
        THUMB
FlashBreak:
        B       FlashBreak

FlashInitEntry:
        BL      mem_region_init
        BL      FlashBreak

FlashSignoffEntry:
        BL      mem_region_deinit
        BL      FlashBreak

FlashWriteEntry:
        BL      mem_region_program
        BL      FlashBreak

FlashEraseWriteEntry:
        BL      mem_region_erase_program
        BL      FlashBreak

FlashChecksumEntry:
        BL      mem_region_verify
        BL      FlashBreak

        SECTION LOWEND:DATA(8)
        DATA
FlashBufferStart:

        SECTION HIGHSTART:DATA
        DATA
FlashBufferEnd:

        PUBLIC  __vector_table
        SECTION .intvec:CODE:ROOT(2)
        DATA
__vector_table:
        DC32    SFE(CSTACK)
        DC32    FlashInitEntry
        DC32    0
        DC32    0
        DC32    0
        DC32    0
        DC32    0
        DC32    0

        END
