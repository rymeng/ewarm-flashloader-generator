#include "framework.h"

/* Import functions for erasure or programming */
#include "algorithm.h"

/* Import functions for debug */
#ifdef DEBUG
#include "board.h"
#endif /* DEBUG */

__root __no_init struct mem_region_handler theFlashParams;

void mem_region_init(void)
{
#ifdef DEBUG
    board_uart_init();
#endif /* DEBUG */

#ifdef DEBUG
    printf("Enter function: FlashInitEntry().\r\n");
    printf("theFlashParams.base = 0x%08lX.\r\n", theFlashParams.base);
    printf("theFlashParams.count = 0x%08lX.\r\n", theFlashParams.count);
    printf("theFlashParams.offset = 0x%08lX.\r\n", theFlashParams.offset);
    printf("theFlashParams.size = 0x%08lX.\r\n", theFlashParams.size);
#endif /* DEBUG */

    theFlashParams.status = __mem_region_init(theFlashParams.base);

#ifdef DEBUG
    printf("Exit function: FlashInitEntry(). ");
    printf("Return status = 0x%08X.\r\n", theFlashParams.status);
    printf("\r\n");
#endif /* DEBUG */

    return;
}

void mem_region_deinit(void)
{
#ifdef DEBUG
    printf("Enter function: FlashSignoffEntry().\r\n");
    printf("theFlashParams.base = 0x%08lX.\r\n", theFlashParams.base);
    printf("theFlashParams.count = 0x%08lX.\r\n", theFlashParams.count);
    printf("theFlashParams.offset = 0x%08lX.\r\n", theFlashParams.offset);
    printf("theFlashParams.size = 0x%08lX.\r\n", theFlashParams.size);
#endif /* DEBUG */

    theFlashParams.status = __mem_region_deinit();

#ifdef DEBUG
    printf("Exit function: FlashSignoffEntry(). ");
    printf("Return status = 0x%08X.\r\n", theFlashParams.status);
    printf("\r\n");
#endif /* DEBUG */

#ifdef DEBUG
    board_uart_deinit();
#endif /* DEBUG */

    return;
}

void mem_region_program(void)
{
#ifdef DEBUG
    printf("Enter function: FlashWriteEntry().\r\n");
    printf("theFlashParams.base = 0x%08lX.\r\n", theFlashParams.base);
    printf("theFlashParams.count = 0x%08lX.\r\n", theFlashParams.count);
    printf("theFlashParams.offset = 0x%08lX.\r\n", theFlashParams.offset);
    printf("theFlashParams.size = 0x%08lX.\r\n", theFlashParams.size);
#endif /* DEBUG */

    theFlashParams.status = __mem_region_program(
        theFlashParams.base + theFlashParams.offset,
        theFlashParams.count,
        (uint8_t *)theFlashParams.buffer
    );

#ifdef DEBUG
    printf("Exit function: FlashWriteEntry(). ");
    printf("Return status = 0x%08X.\r\n", theFlashParams.status);
    printf("\r\n");
#endif /* DEBUG */

    return;
}

void mem_region_erase_program(void)
{
#ifdef DEBUG
    printf("Enter function: FlashEraseWriteEntry().\r\n");
    printf("theFlashParams.base = 0x%08lX.\r\n", theFlashParams.base);
    printf("theFlashParams.count = 0x%08lX.\r\n", theFlashParams.count);
    printf("theFlashParams.offset = 0x%08lX.\r\n", theFlashParams.offset);
    printf("theFlashParams.buffer.base = 0x%08lX.\r\n",
        ((struct mem_region_erase_sector *)theFlashParams.buffer)->base);
    printf("theFlashParams.buffer.size = 0x%08lX.\r\n",
        ((struct mem_region_erase_sector *)theFlashParams.buffer)->size);
    printf("theFlashParams.size = 0x%08lX.\r\n", theFlashParams.size);
#endif /* DEBUG */

    uint32_t status = RESULT_OK;
    uint32_t size = theFlashParams.size;

    if (size == 0UL) {
        struct mem_region_erase_sector *erase_sector =
            (struct mem_region_erase_sector *)theFlashParams.buffer;
        for (uint32_t i = 0UL; i < theFlashParams.count; ++i) {
            status = __mem_region_erase_sector(erase_sector->base);
            if (status != RESULT_OK) {
                break;
            }
            ++erase_sector;
        }
    } else {
        status = __mem_region_erase_sector(theFlashParams.base);
        if (status == RESULT_OK) {
            status = __mem_region_program(
                theFlashParams.base + theFlashParams.offset,
                theFlashParams.count,
                (uint8_t *)theFlashParams.buffer);
        }
    }

    theFlashParams.status = status;

#ifdef DEBUG
    printf("Exit function: FlashEraseWriteEntry(). ");
    printf("Return status = 0x%08X.\r\n", theFlashParams.status);
    printf("\r\n");
#endif /* DEBUG */

    return;
}

void mem_region_verify(void)
{
#ifdef DEBUG
    printf("Enter function: FlashChecksumEntry().\r\n");
    printf("theFlashParams.base = 0x%08lX.\r\n", theFlashParams.base);
    printf("theFlashParams.count = 0x%08lX.\r\n", theFlashParams.count);
    printf("theFlashParams.offset = 0x%08lX.\r\n", theFlashParams.offset);
    printf("theFlashParams.size = 0x%08lX.\r\n", theFlashParams.size);
#endif /* DEBUG */

    theFlashParams.status = __mem_region_verify(
        theFlashParams.base,
        theFlashParams.count
    );

#ifdef DEBUG
    printf("Exit function: FlashChecksumEntry(). ");
    printf("Return status = 0x%08X.\r\n", theFlashParams.status);
    printf("\r\n");
#endif /* DEBUG */

    return;
}
