        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .text:CODE:ROOT(2)
        PUBLIC  FlashInitEntry
        PUBLIC  FlashWriteEntry
        PUBLIC  FlashEraseWriteEntry
        PUBLIC  FlashChecksumEntry
        PUBLIC  FlashSignoffEntry
        EXTERN  init
        EXTERN  program
        EXTERN  erase_program
        EXTERN  verify
        EXTERN  deinit
        THUMB
FlashBreak:
        B       FlashBreak

FlashInitEntry:
        BL      init
        BL      FlashBreak

FlashSignoffEntry:
        BL      deinit
        BL      FlashBreak

FlashWriteEntry:
        BL      program
        BL      FlashBreak

FlashEraseWriteEntry:
        BL      erase_program
        BL      FlashBreak

FlashChecksumEntry:
        BL      verify
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
