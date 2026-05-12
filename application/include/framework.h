#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdint.h>
#include <string.h>

#define RESULT_OK            (0UL)
#define RESULT_ERROR         (1UL)

#define ALIGN(SIZE, BASE)    (    \
    ((SIZE) + (BASE) - 1UL) & ~((BASE) - 1U))

struct mem_region_erase_sector {
    uint32_t base;
    uint32_t size;
};

struct mem_region_handler {
    uint32_t base;
    union {
        uint32_t count;
        uint32_t status;
    };
    uint32_t offset;
    void *buffer;
    uint32_t size;
};

extern void mem_region_init(void);
extern void mem_region_deinit(void);
extern void mem_region_program(void);
extern void mem_region_erase_program(void);
extern void mem_region_verify(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FRAMEWORK_H */
