#ifndef ALGORITHM_H
#define ALGORITHM_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include "rt32f7.h"

#define ERASE_SIZE                                       (1024UL)
#define PROGRAM_SIZE                                     (8UL)

#define FLASH_BANK1_MAIN_BLOCK_BASE_ADDR                 (0x08000000UL)
#define FLASH_BANK1_MAIN_BLOCK_SIZE                      (0x00080000UL)

#define FLASH_BANK2_MAIN_BLOCK_BASE_ADDR                 (0x08080000UL)
#define FLASH_BANK2_MAIN_BLOCK_SIZE                      (0x00080000UL)

#define ITCM_BASE_ADDR                                   (0x00000000UL)
#define ITCM_SIZE                                        (0x00020000UL)

#define DTCM_BASE_ADDR                                   (0x20000000UL)
#define DTCM_SIZE                                        (0x00020000UL)

#define SRAM1_BASE_ADDR                                  (0x20100000UL)
#define SRAM1_SIZE                                       (0x00020000UL)

#define SRAM2_BASE_ADDR                                  (0x20120000UL)
#define SRAM2_SIZE                                       (0x00020000UL)

#define FLASH_BANK1_MAIN_BLOCK_END_ADDR                  (    \
    FLASH_BANK1_MAIN_BLOCK_BASE_ADDR +                        \
    FLASH_BANK1_MAIN_BLOCK_SIZE -                             \
    0x1UL)
#define FLASH_BANK1_MAIN_BLOCK_LAST_ERASE_UNIT_ADDR      (    \
    FLASH_BANK1_MAIN_BLOCK_BASE_ADDR +                        \
    FLASH_BANK1_MAIN_BLOCK_SIZE -                             \
    ERASE_SIZE)
#define FLASH_BANK1_MAIN_BLOCK_LAST_PROGRAM_UNIT_ADDR    (    \
    FLASH_BANK1_MAIN_BLOCK_BASE_ADDR +                        \
    FLASH_BANK1_MAIN_BLOCK_SIZE -                             \
    PROGRAM_SIZE)

#define FLASH_BANK2_MAIN_BLOCK_END_ADDR                  (    \
    FLASH_BANK2_MAIN_BLOCK_BASE_ADDR +                        \
    FLASH_BANK2_MAIN_BLOCK_SIZE -                             \
    0x1UL)
#define FLASH_BANK2_MAIN_BLOCK_LAST_ERASE_UNIT_ADDR      (    \
    FLASH_BANK2_MAIN_BLOCK_BASE_ADDR +                        \
    FLASH_BANK2_MAIN_BLOCK_SIZE -                             \
    ERASE_SIZE)
#define FLASH_BANK2_MAIN_BLOCK_LAST_PROGRAM_UNIT_ADDR    (    \
    FLASH_BANK2_MAIN_BLOCK_BASE_ADDR +                        \
    FLASH_BANK2_MAIN_BLOCK_SIZE -                             \
    PROGRAM_SIZE)

#define ITCM_END_ADDR                                    (    \
    ITCM_BASE_ADDR +                                          \
    ITCM_SIZE -                                               \
    0x1UL)
#define ITCM_LAST_ERASE_UNIT_ADDR                        (    \
    ITCM_BASE_ADDR +                                          \
    ITCM_SIZE -                                               \
    ERASE_SIZE)
#define ITCM_LAST_PROGRAM_UNIT_ADDR                      (    \
    ITCM_BASE_ADDR +                                          \
    ITCM_SIZE -                                               \
    PROGRAM_SIZE)

#define DTCM_END_ADDR                                    (    \
    DTCM_BASE_ADDR +                                          \
    DTCM_SIZE -                                               \
    0x1UL)
#define DTCM_LAST_ERASE_UNIT_ADDR                        (    \
    DTCM_BASE_ADDR +                                          \
    DTCM_SIZE -                                               \
    ERASE_SIZE)
#define DTCM_LAST_PROGRAM_UNIT_ADDR                      (    \
    DTCM_BASE_ADDR +                                          \
    DTCM_SIZE -                                               \
    PROGRAM_SIZE)

#define SRAM1_END_ADDR                                   (    \
    SRAM1_BASE_ADDR +                                         \
    SRAM1_SIZE -                                              \
    0x1UL)
#define SRAM1_LAST_ERASE_UNIT_ADDR                       (    \
    SRAM1_BASE_ADDR +                                         \
    SRAM1_SIZE -                                              \
    ERASE_SIZE)
#define SRAM1_LAST_PROGRAM_UNIT_ADDR                     (    \
    SRAM1_BASE_ADDR +                                         \
    SRAM1_SIZE -                                              \
    PROGRAM_SIZE)

#define SRAM2_END_ADDR                                   (    \
    SRAM2_BASE_ADDR +                                         \
    SRAM2_SIZE -                                              \
    0x1UL)
#define SRAM2_LAST_ERASE_UNIT_ADDR                       (    \
    SRAM2_BASE_ADDR +                                         \
    SRAM2_SIZE -                                              \
    ERASE_SIZE)
#define SRAM2_LAST_PROGRAM_UNIT_ADDR                     (    \
    SRAM2_BASE_ADDR +                                         \
    SRAM2_SIZE -                                              \
    PROGRAM_SIZE)

#define FMC_KEY1                                         (0x45670123UL)
#define FMC_KEY2                                         (0xCDEF89ABUL)

enum flash_latency {
    FLASH_LATENCY_0_WAIT_STATE = 0,
    FLASH_LATENCY_1_WAIT_STATE = 1,
    FLASH_LATENCY_2_WAIT_STATE = 2,
    FLASH_LATENCY_3_WAIT_STATE = 3,
    FLASH_LATENCY_4_WAIT_STATE = 4
};

extern uint32_t __mem_region_init(uint32_t address);
extern uint32_t __mem_region_deinit(void);
extern uint32_t __mem_region_erase_sector(uint32_t address);
extern uint32_t __mem_region_program(uint32_t address, uint32_t size,
                                     uint8_t *buffer);
extern uint32_t __mem_region_verify(uint32_t address, uint32_t size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALGORITHM_H */
