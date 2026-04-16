#include "framework.h"

#include "device_algorithm.h"
#include "rt32f7.h"

static uint16_t crc16(uint8_t *buffer, uint32_t size);
static uint16_t __crc16(uint8_t *buffer, uint32_t size, uint16_t initial_value);

#pragma diag_suppress=Pe186

uint32_t __init(uint32_t address)
{
    uint32_t ret = RESULT_OK;

    if ((address >= FLASH_BANK1_BASE) && (address <= FLASH_BANK1_END)) {
        /* Unlock FMC 1 */
        FLASH->BANK1KR.reg = FMC_KEY1;
        __DSB();
        FLASH->BANK1KR.reg = FMC_KEY2;
        __DSB();

        /* Check FMC 1 lock status */
        if (FLASH->BANK1CR.reg & FLASH_BANK1CR_LOCK_Msk) {
            ret = RESULT_ERROR;
        }
    } else if ((address >= FLASH_BANK2_BASE) && (address <= FLASH_BANK2_END)) {
        /* Unlock FMC 2 */
        FLASH->BANK2KR.reg = FMC_KEY1;
        __DSB();
        FLASH->BANK2KR.reg = FMC_KEY2;
        __DSB();

        /* Check FMC 2 lock status */
        if (FLASH->BANK2CR.reg & FLASH_BANK2CR_LOCK_Msk) {
            ret = RESULT_ERROR;
        }
    } else if ((address >= ITCM_BASE) && (address <= ITCM_END)) {
        ;
    } else if ((address >= DTCM_BASE) && (address <= DTCM_END)) {
        ;
    } else if ((address >= SRAM1_BASE) && (address <= SRAM1_END)) {
        ;
    } else if ((address >= SRAM2_BASE) && (address <= SRAM2_END)) {
        ;
    } else {
        ret = RESULT_ERROR;
    }

    return ret;
}

uint32_t __deinit(void)
{
    uint32_t ret = RESULT_OK;

    /* Lock FMC 1 & FMC 2 */
    FLASH->BANK1CR.reg |= FLASH_BANK1CR_LOCK_Msk;
    FLASH->BANK2CR.reg |= FLASH_BANK2CR_LOCK_Msk;
    __DSB();

    /* Check FMC 1 & FMC 2 lock status */
    if (!(FLASH->BANK1CR.reg & FLASH_BANK1CR_LOCK_Msk) ||
        !(FLASH->BANK2CR.reg & FLASH_BANK2CR_LOCK_Msk)) {
        ret = RESULT_ERROR;
    }

    return ret;
}

uint32_t __erase_sector(uint32_t address)
{
    uint32_t ret = RESULT_OK;

    if ((address >= FLASH_BANK1_BASE) &&
        (address <= FLASH_BANK1_LAST_ERASE_UNIT)) {
        /* Clear EOP flag */
        FLASH->BANK1SR.reg |= FLASH_BANK1SR_EOP_Msk;
        __DSB();

        /* Write destination address */
        FLASH->BANK1AR.reg = address;

        /* Enable erasing sectors */
        FLASH->BANK1CR.reg |= FLASH_BANK1CR_SER_EN_Msk;
        __DSB();

        /* Start erasing */
        FLASH->BANK1CR.reg |= FLASH_BANK1CR_START_Msk;

        /* Wait for erasing start */
        while (FLASH->BANK1CR.reg & FLASH_BANK1CR_START_Msk) {
            ;
        }

        /* Wait for erasing done */
        while (FLASH->BANK1SR.reg & FLASH_BANK1SR_BUSY_Msk) {
            ;
        }

        /* Disable erasing sectors */
        FLASH->BANK1CR.reg &= ~FLASH_BANK1CR_SER_EN_Msk;
        __DSB();

        /* Check FMC 1 EOP flag */
        if (FLASH->BANK1SR.reg & FLASH_BANK1SR_EOP_Msk) {
            /* Clear FMC 1 EOP flag */
            FLASH->BANK1SR.reg |= FLASH_BANK1SR_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= FLASH_BANK2_BASE) &&
               (address <= FLASH_BANK2_LAST_ERASE_UNIT)) {
        /* Clear EOP flag */
        FLASH->BANK2SR.reg |= FLASH_BANK2SR_EOP_Msk;
        __DSB();

        /* Write destination address */
        FLASH->BANK2AR.reg = address;

        /* Enable erasing sectors */
        FLASH->BANK2CR.reg |= FLASH_BANK2CR_SER_EN_Msk;
        __DSB();

        /* Start erasing */
        FLASH->BANK2CR.reg |= FLASH_BANK2CR_START_Msk;

        /* Wait for erasing start */
        while (FLASH->BANK2CR.reg & FLASH_BANK2CR_START_Msk) {
            ;
        }

        /* Wait for erasing done */
        while (FLASH->BANK2SR.reg & FLASH_BANK2SR_BUSY_Msk) {
            ;
        }

        /* Disable erasing sectors */
        FLASH->BANK2CR.reg &= ~FLASH_BANK2CR_SER_EN_Msk;
        __DSB();

        /* Check FMC 2 EOP flag */
        if (FLASH->BANK2SR.reg & FLASH_BANK2SR_EOP_Msk) {
            /* Clear FMC 2 EOP flag */
            FLASH->BANK2SR.reg |= FLASH_BANK2SR_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= ITCM_BASE) &&
               (address <= ITCM_LAST_ERASE_UNIT)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else if ((address >= DTCM_BASE) &&
               (address <= DTCM_LAST_ERASE_UNIT)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else if ((address >= SRAM1_BASE) &&
               (address <= SRAM1_LAST_ERASE_UNIT)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else if ((address >= SRAM2_BASE) &&
               (address <= SRAM2_LAST_ERASE_UNIT)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else {
        ret = RESULT_ERROR;
    }

    return ret;
}

uint32_t __program(uint32_t address, uint32_t size, uint8_t *buffer)
{
    uint32_t ret = RESULT_OK;

    if ((address >= FLASH_BANK1_BASE) &&
        (address <= FLASH_BANK1_LAST_PROGRAM_UNIT)) {
        /* Data filling */
        uint32_t aligned_size = ALIGN(size, PROGRAM_SIZE);
        if (aligned_size != size) {
            memset(buffer + size, 0xFF, aligned_size - size);
            size = aligned_size;
        }

        /* Enable programming */
        FLASH->BANK1CR.reg |= FLASH_BANK1CR_PG_EN_Msk;
        __DSB();

        /* Program */
        for (uint32_t i = 0UL; i < size; i += PROGRAM_SIZE) {
            for (uint32_t j = 0UL; j < PROGRAM_SIZE; j += 8UL) {
                *(uint64_t *)(address + i + j) = *(uint64_t *)(buffer + i + j);
                __DSB();
            }

            /* Wait for programming done */
            while (FLASH->BANK1SR.reg & FLASH_BANK1SR_BUSY_Msk) {
                ;
            }
        }

        /* Disable programming */
        FLASH->BANK1CR.reg &= ~FLASH_BANK1CR_PG_EN_Msk;
        __DSB();

        /* Check FMC 1 EOP flag */
        if (FLASH->BANK1SR.reg & FLASH_BANK1SR_EOP_Msk) {
            /* Clear FMC 1 EOP flag */
            FLASH->BANK1SR.reg |= FLASH_BANK1SR_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= FLASH_BANK2_BASE) &&
               (address <= FLASH_BANK2_LAST_PROGRAM_UNIT)) {
        /* Data filling */
        uint32_t aligned_size = ALIGN(size, PROGRAM_SIZE);
        if (aligned_size != size) {
            memset(buffer + size, 0xFF, aligned_size - size);
            size = aligned_size;
        }

        /* Enable programming */
        FLASH->BANK2CR.reg |= FLASH_BANK2CR_PG_EN_Msk;
        __DSB();

        /* Program */
        for (uint32_t i = 0UL; i < size; i += PROGRAM_SIZE) {
            for (uint32_t j = 0UL; j < PROGRAM_SIZE; j += 8UL) {
                *(uint64_t *)(address + i + j) = *(uint64_t *)(buffer + i + j);
                __DSB();
            }

            /* Wait for programming done */
            while (FLASH->BANK2SR.reg & FLASH_BANK2SR_BUSY_Msk) {
                ;
            }
        }

        /* Disable programming */
        FLASH->BANK2CR.reg &= ~FLASH_BANK2CR_PG_EN_Msk;
        __DSB();

        /* Check FMC 2 EOP flag */
        if (FLASH->BANK2SR.reg & FLASH_BANK2SR_EOP_Msk) {
            /* Clear FMC 2 EOP flag */
            FLASH->BANK2SR.reg |= FLASH_BANK2SR_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= ITCM_BASE) &&
               (address <= ITCM_LAST_PROGRAM_UNIT)) {
        for (uint32_t i = 0UL; i < size; ++i) {
            *(volatile uint8_t *)(address + i) = *(buffer + i);
        }
    } else if ((address >= DTCM_BASE) &&
               (address <= DTCM_LAST_PROGRAM_UNIT)) {
        for (uint32_t i = 0UL; i < size; ++i) {
            *(volatile uint8_t *)(address + i) = *(buffer + i);
        }
    } else if ((address >= SRAM1_BASE) &&
               (address <= SRAM1_LAST_PROGRAM_UNIT)) {
        for (uint32_t i = 0UL; i < size; ++i) {
            *(volatile uint8_t *)(address + i) = *(buffer + i);
        }
    } else if ((address >= SRAM2_BASE) &&
               (address <= SRAM2_LAST_PROGRAM_UNIT)) {
        for (uint32_t i = 0UL; i < size; ++i) {
            *(volatile uint8_t *)(address + i) = *(buffer + i);
        }
    } else {
        ret = RESULT_ERROR;
    }

    return ret;
}

uint32_t __verify(uint32_t address, uint32_t size)
{
    uint8_t *buffer = (uint8_t *)address;

    return crc16(buffer, size);
}

static uint16_t crc16(uint8_t *buffer, uint32_t size)
{
    uint8_t data_tail[2UL] = {0U, 0U};
    uint16_t result = 0U;

    result = __crc16(buffer, size, result);
    result = __crc16(data_tail, 2UL, result);

    return result;
}

static uint16_t __crc16(uint8_t *buffer, uint32_t size, uint16_t initial_value)
{
    uint16_t result = initial_value;

    while (size--) {
        uint8_t current_byte = *buffer;
        for (uint32_t i = 0UL; i < 8UL; ++i) {
            uint16_t last_result = result;
            result <<= 1UL;
            if (current_byte & 0x80U) {
                result |= 0x1U;
            }
            if (last_result & 0x8000U) {
                result ^= 0x1021U;
            }
            current_byte <<= 1UL;
        }
        ++buffer;
    }

    return result;
}

#pragma diag_default=Pe186
