#include "framework.h"

#include "device_algorithm.h"

__root const uint16_t frameworkVersion = 200U;
__root __no_init struct device_handler theFlashParams;

void init(void)
{
    theFlashParams.status = __init(theFlashParams.base);

    return;
}

void deinit(void)
{
    theFlashParams.status = __deinit();

    return;
}

void program(void)
{
    theFlashParams.status = __program(
        theFlashParams.base + theFlashParams.offset,
        theFlashParams.count,
        (uint8_t *)theFlashParams.buffer
    );

    return;
}

void erase_program(void)
{
    uint32_t status = RESULT_OK;
    uint32_t size = theFlashParams.size;

    if (size == 0UL) {
        struct device_erase_buffer_info *device_erase_buffer =
            (struct device_erase_buffer_info *)theFlashParams.buffer;
        for (uint32_t i = 0UL; i < theFlashParams.count; ++i) {
            status = __erase_sector(device_erase_buffer->base);
            if (status != RESULT_OK) {
                break;
            }
            ++device_erase_buffer;
        }
    } else {
        status = __erase_sector(theFlashParams.base);
        if (status == RESULT_OK) {
            status = __program(
                theFlashParams.base + theFlashParams.offset,
                theFlashParams.count,
                (uint8_t *)theFlashParams.buffer);
        }
    }

    theFlashParams.status = status;

    return;
}

void verify(void)
{
    theFlashParams.status = __verify(
        theFlashParams.base,
        theFlashParams.count
    );

    return;
}
