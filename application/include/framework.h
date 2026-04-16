#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <string.h>

#define RESULT_OK                   (0UL)
#define RESULT_ERROR                (1UL)

#define ALIGN(SIZE, BASE)           (((SIZE) + (BASE) - 1UL) & ~((BASE) - 1U))

struct device_erase_buffer_info {
    uint32_t base;
    uint32_t size;
};

struct device_handler {
    uint32_t base;
    union {
        uint32_t count;
        uint32_t status;
    };
    uint32_t offset;
    void *buffer;
    uint32_t size;
};

extern void init(void);
extern void deinit(void);
extern void program(void);
extern void erase_program(void);
extern void verify(void);

#ifdef __cplusplus
}
#endif

#endif /* FRAMEWORK_H */
