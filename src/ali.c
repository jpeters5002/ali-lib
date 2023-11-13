#include "ali.h"
#include "ali_err.h"

#include <stdlib.h>

void ali_deinit(struct ali *self) {
    free(self->_number);
}

#define RATIO_PRIMITIVE_TO_SIZE_T_RU(primitive) \
    ((sizeof(primitive) / sizeof(size_t)) ? (sizeof(primitive) / sizeof(size_t)) : 1)

#define DEFINE_PRIMITIVE_ALI_SET_VALUE(type) \
    /*ali_err_t ali_set_value_##typename(struct ali *self, type val) {*/ \
    if (self->_number == NULL) { \
        self->_number = malloc(sizeof(size_t) * RATIO_PRIMITIVE_TO_SIZE_T_RU(type)); \
        if (self->_number == NULL) { \
            /* malloc fail */ \
            return eALI_ERR_ALLOCFAIL; \
        } \
    } else if (self->_len != RATIO_PRIMITIVE_TO_SIZE_T_RU(type)) { \
        void *tmp = realloc(self->_number, sizeof(size_t) * RATIO_PRIMITIVE_TO_SIZE_T_RU(type)); \
        if (tmp == NULL) { \
            /* realloc fail */ \
            return eALI_ERR_ALLOCFAIL; \
        } \
        self->_number = tmp; \
    } \
    self->_len = RATIO_PRIMITIVE_TO_SIZE_T_RU(type); \
    for (size_t i = 0; i < RATIO_PRIMITIVE_TO_SIZE_T_RU(type); i++) { \
        /* reverse index access; set to the value shifted properly and casted (potentially down) to a size_t */ \
        self->_number[RATIO_PRIMITIVE_TO_SIZE_T_RU(type) - 1 - i] = (size_t) (val >> (8 * i * sizeof(size_t))); \
    } \
    return eALI_ERR_NOERR; \
    /*}*/

ali_err_t ali_set_value_u64(struct ali *self, uint64_t val) {
    DEFINE_PRIMITIVE_ALI_SET_VALUE(uint64_t)
}
ali_err_t ali_set_value_u32(struct ali *self, uint32_t val) {
    DEFINE_PRIMITIVE_ALI_SET_VALUE(uint32_t)
}
ali_err_t ali_set_value_u16(struct ali *self, uint16_t val) {
    DEFINE_PRIMITIVE_ALI_SET_VALUE(uint16_t)
}
ali_err_t ali_set_value_u8(struct ali *self, uint8_t val) {
    DEFINE_PRIMITIVE_ALI_SET_VALUE(uint8_t)
}

