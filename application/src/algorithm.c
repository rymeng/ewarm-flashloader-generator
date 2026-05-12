#include "algorithm.h"

/* Import framework macros */
#include "framework.h"

static void flash_latency_init(void);
static void flash_latency_deinit(void);
static uint16_t crc16(uint8_t *buffer, uint32_t size);
static uint16_t __crc16(uint8_t *buffer, uint32_t size, uint16_t initial_value);

__root __no_init enum flash_latency user_application_flash_latency;

#pragma diag_suppress=Pe186

uint32_t __mem_region_init(uint32_t address)
{
    uint32_t ret = RESULT_OK;

    /* Initialize flash latency */
    flash_latency_init();
    if ((address >= FLASH_BANK1_MAIN_BLOCK_BASE_ADDR) &&
	    (address <= FLASH_BANK1_MAIN_BLOCK_END_ADDR)) {
        /* Unlock FMC 1 */
        FLASH->UNLOCKR1.reg = FMC_KEY1;
        __DSB();
        FLASH->UNLOCKR1.reg = FMC_KEY2;
        __DSB();

        /* Check FMC 1 lock status */
        if (FLASH->CR1.reg & FLASH_CR1_LOCK_Msk) {
            ret = RESULT_ERROR;
        }
    } else if ((address >= FLASH_BANK2_MAIN_BLOCK_BASE_ADDR) &&
	           (address <= FLASH_BANK2_MAIN_BLOCK_END_ADDR)) {
        /* Unlock FMC 2 */
        FLASH->UNLOCKR2.reg = FMC_KEY1;
        __DSB();
        FLASH->UNLOCKR2.reg = FMC_KEY2;
        __DSB();

        /* Check FMC 2 lock status */
        if (FLASH->CR2.reg & FLASH_CR2_LOCK_Msk) {
            ret = RESULT_ERROR;
        }
    } else if ((address >= ITCM_BASE_ADDR) && (address <= ITCM_END_ADDR)) {
        ;
    } else if ((address >= DTCM_BASE_ADDR) && (address <= DTCM_END_ADDR)) {
        ;
    } else if ((address >= SRAM1_BASE_ADDR) && (address <= SRAM1_END_ADDR)) {
        ;
    } else {
        ret = RESULT_ERROR;
    }

    return ret;
}

uint32_t __mem_region_deinit(void)
{
    uint32_t ret = RESULT_OK;

    /* Lock FMC 1 & FMC 2 */
    FLASH->CR1.reg |= FLASH_CR1_LOCK_Msk;
    FLASH->CR2.reg |= FLASH_CR2_LOCK_Msk;
    __DSB();

    /* Check FMC 1 & FMC 2 lock status */
    if (!(FLASH->CR1.reg & FLASH_CR1_LOCK_Msk) ||
        !(FLASH->CR2.reg & FLASH_CR2_LOCK_Msk)) {
        ret = RESULT_ERROR;
    }

    /* De-initialize flash latency */
    flash_latency_deinit();

    return ret;
}

uint32_t __mem_region_erase_sector(uint32_t address)
{
    uint32_t ret = RESULT_OK;

    if ((address >= FLASH_BANK1_MAIN_BLOCK_BASE_ADDR) &&
        (address <= FLASH_BANK1_MAIN_BLOCK_LAST_ERASE_UNIT_ADDR)) {
        /* Clear EOP flag */
        FLASH->SR1.reg |= FLASH_SR1_EOP_Msk;
        __DSB();

        /* Write destination address */
        FLASH->AR1.reg = address;

        /* Enable sector erasure */
        FLASH->CR1.reg |= FLASH_CR1_SEREN_Msk;
        __DSB();

        /* Start erasing */
        FLASH->CR1.reg |= FLASH_CR1_START_Msk;

        /* Wait for erasing start */
        while (FLASH->CR1.reg & FLASH_CR1_START_Msk) {
            ;
        }

        /* Wait for erasing done */
        while (FLASH->SR1.reg & FLASH_SR1_BUSY_Msk) {
            ;
        }

        /* Disable sector erasure */
        FLASH->CR1.reg &= ~FLASH_CR1_SEREN_Msk;
        __DSB();

        /* Check FMC 1 EOP flag */
        if (FLASH->SR1.reg & FLASH_SR1_EOP_Msk) {
            /* Clear FMC 1 EOP flag */
            FLASH->SR1.reg |= FLASH_SR1_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= FLASH_BANK1_MAIN_BLOCK_BASE_ADDR) &&
               (address <= FLASH_BANK2_MAIN_BLOCK_LAST_ERASE_UNIT_ADDR)) {
        /* Clear EOP flag */
        FLASH->SR2.reg |= FLASH_SR2_EOP_Msk;
        __DSB();

        /* Write destination address */
        FLASH->AR2.reg = address;

        /* Enable sector erasure */
        FLASH->CR2.reg |= FLASH_CR2_SEREN_Msk;
        __DSB();

        /* Start erasing */
        FLASH->CR2.reg |= FLASH_CR2_START_Msk;

        /* Wait for erasing start */
        while (FLASH->CR2.reg & FLASH_CR2_START_Msk) {
            ;
        }

        /* Wait for erasing done */
        while (FLASH->SR2.reg & FLASH_SR2_BUSY_Msk) {
            ;
        }

        /* Disable sector erasure */
        FLASH->CR2.reg &= ~FLASH_CR2_SEREN_Msk;
        __DSB();

        /* Check FMC 2 EOP flag */
        if (FLASH->SR2.reg & FLASH_SR2_EOP_Msk) {
            /* Clear FMC 2 EOP flag */
            FLASH->SR2.reg |= FLASH_SR2_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= ITCM_BASE_ADDR) &&
               (address <= ITCM_LAST_ERASE_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else if ((address >= DTCM_BASE_ADDR) &&
               (address <= DTCM_LAST_ERASE_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else if ((address >= SRAM1_BASE_ADDR) &&
               (address <= SRAM1_LAST_ERASE_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < ERASE_SIZE / 4UL; i+= 4UL) {
            *(volatile uint32_t *)(address + i) = 0xFFFFFFFFUL;
        }
    } else {
        ret = RESULT_ERROR;
    }

    return ret;

}

uint32_t __mem_region_program(uint32_t address, uint32_t size, uint8_t *buffer)
{
    uint32_t ret = RESULT_OK;

    if ((address >= FLASH_BANK1_MAIN_BLOCK_BASE_ADDR) &&
        (address <= FLASH_BANK1_MAIN_BLOCK_LAST_PROGRAM_UNIT_ADDR)) {
        /* Data filling */
        uint32_t aligned_size = ALIGN(size, PROGRAM_SIZE);
        if (aligned_size != size) {
            memset(buffer + size, 0xFF, aligned_size - size);
            size = aligned_size;
        }

        /* Enable programming */
        FLASH->CR1.reg |= FLASH_CR1_PGEN_Msk;
        __DSB();

        /* Program */
        for (uint32_t i = 0UL; i < size; i += PROGRAM_SIZE) {
            for (uint32_t j = 0UL; j < PROGRAM_SIZE; j += 8UL) {
                *(uint64_t *)(address + i + j) = *(uint64_t *)(buffer + i + j);
                __DSB();
            }

            /* Wait for programming done */
            while (FLASH->SR1.reg & FLASH_SR1_BUSY_Msk) {
                ;
            }
        }

        /* Disable programming */
        FLASH->CR1.reg &= ~FLASH_CR1_PGEN_Msk;
        __DSB();

        /* Check FMC 1 EOP flag */
        if (FLASH->SR1.reg & FLASH_SR1_EOP_Msk) {
            /* Clear FMC 1 EOP flag */
            FLASH->SR1.reg |= FLASH_SR1_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= FLASH_BANK2_MAIN_BLOCK_BASE_ADDR) &&
               (address <= FLASH_BANK2_MAIN_BLOCK_LAST_PROGRAM_UNIT_ADDR)) {
        /* Data filling */
        uint32_t aligned_size = ALIGN(size, PROGRAM_SIZE);
        if (aligned_size != size) {
            memset(buffer + size, 0xFF, aligned_size - size);
            size = aligned_size;
        }

        /* Enable programming */
        FLASH->CR2.reg |= FLASH_CR2_PGEN_Msk;
        __DSB();

        /* Program */
        for (uint32_t i = 0UL; i < size; i += PROGRAM_SIZE) {
            for (uint32_t j = 0UL; j < PROGRAM_SIZE; j += 8UL) {
                *(uint64_t *)(address + i + j) = *(uint64_t *)(buffer + i + j);
                __DSB();
            }

            /* Wait for programming done */
            while (FLASH->SR2.reg & FLASH_SR2_BUSY_Msk) {
                ;
            }
        }

        /* Disable programming */
        FLASH->CR2.reg &= ~FLASH_CR2_PGEN_Msk;
        __DSB();

        /* Check FMC 2 EOP flag */
        if (FLASH->SR2.reg & FLASH_SR2_EOP_Msk) {
            /* Clear FMC 2 EOP flag */
            FLASH->SR2.reg |= FLASH_SR2_EOP_Msk;
            __DSB();
        } else {
            ret = RESULT_ERROR;
        }
    } else if ((address >= ITCM_BASE_ADDR) &&
               (address <= ITCM_LAST_PROGRAM_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < size; ++i) {
            *(volatile uint8_t *)(address + i) = *(buffer + i);
        }
    } else if ((address >= DTCM_BASE_ADDR) &&
               (address <= DTCM_LAST_PROGRAM_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < size; ++i) {
            *(volatile uint8_t *)(address + i) = *(buffer + i);
        }
    } else if ((address >= SRAM1_BASE_ADDR) &&
               (address <= SRAM1_LAST_PROGRAM_UNIT_ADDR)) {
        for (uint32_t i = 0UL; i < size; ++i) {
            *(volatile uint8_t *)(address + i) = *(buffer + i);
        }
    } else {
        ret = RESULT_ERROR;
    }

    return ret;
}

uint32_t __mem_region_verify(uint32_t address, uint32_t size)
{
    uint8_t *buffer = (uint8_t *)address;

    return crc16(buffer, size);
}

static void flash_latency_init(void)
{
    /* Get and save user application flash latency */
    switch ((FLASH->ACR.reg & FLASH_ACR_LATENCY_Msk) >> FLASH_ACR_LATENCY_Pos) {
        case 0x0UL: {
            user_application_flash_latency = FLASH_LATENCY_0_WAIT_STATE;
            break;
        }
        case 0x1UL: {
            user_application_flash_latency = FLASH_LATENCY_1_WAIT_STATE;
            break;
        }
        case 0x2UL: {
            user_application_flash_latency = FLASH_LATENCY_2_WAIT_STATE;
            break;
        }
        case 0x3UL: {
            user_application_flash_latency = FLASH_LATENCY_3_WAIT_STATE;
            break;
        }
        case 0x4UL: {
            user_application_flash_latency = FLASH_LATENCY_4_WAIT_STATE;
            break;
        }
        default: {
            user_application_flash_latency = FLASH_LATENCY_0_WAIT_STATE;
            break;
        }
    }

    /* Set flash latency: 4 wait state */
    FLASH->ACR.reg &= ~FLASH_ACR_LATENCY_Msk;
    __DSB();
    FLASH->ACR.reg |= 0x4UL << FLASH_ACR_LATENCY_Pos;
    __DSB();

    return;
}

static void flash_latency_deinit(void)
{
    /* Restore user application flash latency */
    switch (user_application_flash_latency) {
        case FLASH_LATENCY_0_WAIT_STATE: {
            FLASH->ACR.reg &= ~FLASH_ACR_LATENCY_Msk;
            __DSB();
            FLASH->ACR.reg |= 0x0UL << FLASH_ACR_LATENCY_Pos;
            __DSB();
            break;
        }
        case FLASH_LATENCY_1_WAIT_STATE: {
            FLASH->ACR.reg &= ~FLASH_ACR_LATENCY_Msk;
            __DSB();
            FLASH->ACR.reg |= 0x1UL << FLASH_ACR_LATENCY_Pos;
            __DSB();
            break;
        }
        case FLASH_LATENCY_2_WAIT_STATE: {
            FLASH->ACR.reg &= ~FLASH_ACR_LATENCY_Msk;
            __DSB();
            FLASH->ACR.reg |= 0x2UL << FLASH_ACR_LATENCY_Pos;
            __DSB();
            break;
        }
        case FLASH_LATENCY_3_WAIT_STATE: {
            FLASH->ACR.reg &= ~FLASH_ACR_LATENCY_Msk;
            __DSB();
            FLASH->ACR.reg |= 0x3UL << FLASH_ACR_LATENCY_Pos;
            __DSB();
            break;
        }
        case FLASH_LATENCY_4_WAIT_STATE: {
            FLASH->ACR.reg &= ~FLASH_ACR_LATENCY_Msk;
            __DSB();
            FLASH->ACR.reg |= 0x4UL << FLASH_ACR_LATENCY_Pos;
            __DSB();
            break;
        }
        default: {
            FLASH->ACR.reg &= ~FLASH_ACR_LATENCY_Msk;
            __DSB();
            FLASH->ACR.reg |= 0x0UL << FLASH_ACR_LATENCY_Pos;
            __DSB();
            break;
        }
    }

    return;
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
