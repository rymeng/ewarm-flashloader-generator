#ifndef DEVICE_ALGORITHM_H
#define DEVICE_ALGORITHM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define PROGRAM_SIZE            (8UL)
#define ERASE_SIZE              (1024UL)

#define FLASH_BANK1_BASE        (0x08000000UL)
#define FLASH_BANK1_SIZE        (0x00080000UL)
#define FLASH_BANK1_END         (FLASH_BANK1_BASE + FLASH_BANK1_SIZE - 0x1UL)
#define FLASH_BANK1_LAST_ERASE_UNIT \
    (FLASH_BANK1_BASE + FLASH_BANK1_SIZE - ERASE_SIZE)
#define FLASH_BANK1_LAST_PROGRAM_UNIT \
    (FLASH_BANK1_BASE + FLASH_BANK1_SIZE - PROGRAM_SIZE)
#define FLASH_BANK2_BASE        (0x08080000UL)
#define FLASH_BANK2_SIZE        (0x00080000UL)
#define FLASH_BANK2_END         (FLASH_BANK2_BASE + FLASH_BANK2_SIZE - 0x1UL)
#define FLASH_BANK2_LAST_ERASE_UNIT \
    (FLASH_BANK2_BASE + FLASH_BANK2_SIZE - ERASE_SIZE)
#define FLASH_BANK2_LAST_PROGRAM_UNIT \
    (FLASH_BANK2_BASE + FLASH_BANK2_SIZE - PROGRAM_SIZE)
#define ITCM_BASE               (0x00000000UL)
#define ITCM_SIZE               (0x00020000UL)
#define ITCM_END                (ITCM_BASE + ITCM_SIZE - 0x1UL)
#define ITCM_LAST_ERASE_UNIT    (ITCM_BASE + ITCM_SIZE - ERASE_SIZE)
#define ITCM_LAST_PROGRAM_UNIT  (ITCM_BASE + ITCM_SIZE - PROGRAM_SIZE)
#define DTCM_BASE               (0x20000000UL)
#define DTCM_SIZE               (0x00020000UL)
#define DTCM_END                (DTCM_BASE + DTCM_SIZE - 0x1UL)
#define DTCM_LAST_ERASE_UNIT    (DTCM_BASE + DTCM_SIZE - ERASE_SIZE)
#define DTCM_LAST_PROGRAM_UNIT  (DTCM_BASE + DTCM_SIZE - PROGRAM_SIZE)
#define SRAM1_BASE              (0x20100000UL)
#define SRAM1_SIZE              (0x00020000UL)
#define SRAM1_END               (SRAM1_BASE + SRAM1_SIZE - 0x1UL)
#define SRAM1_LAST_ERASE_UNIT   (SRAM1_BASE + SRAM1_SIZE - ERASE_SIZE)
#define SRAM1_LAST_PROGRAM_UNIT (SRAM1_BASE + SRAM1_SIZE - PROGRAM_SIZE)
#define SRAM2_BASE              (0x20120000UL)
#define SRAM2_SIZE              (0x00020000UL)
#define SRAM2_END               (SRAM2_BASE + SRAM2_SIZE - 0x1UL)
#define SRAM2_LAST_ERASE_UNIT   (SRAM2_BASE + SRAM2_SIZE - ERASE_SIZE)
#define SRAM2_LAST_PROGRAM_UNIT (SRAM2_BASE + SRAM2_SIZE - PROGRAM_SIZE)

#define FMC_KEY1                (0x45670123UL)
#define FMC_KEY2                (0xCDEF89ABUL)

extern uint32_t __init(uint32_t address);
extern uint32_t __deinit(void);
extern uint32_t __erase_sector(uint32_t address);
extern uint32_t __program(uint32_t address, uint32_t size, uint8_t *buffer);
extern uint32_t __verify(uint32_t address, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* DEVICE_ALGORITHM_H */
