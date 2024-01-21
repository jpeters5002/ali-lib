#include "ali.h"
#include "ali_err.h"

#include <stdlib.h>
#define SIZE_T_MAX ((size_t) - 1)

void ali_init(struct ali *self) {
    *self = ALI_INIT_LIST();
}

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

int my_atoi(const char* val) {
    size_t len = strlen(val);
    int num = 0;

    for (size_t i = 0; i < len; i++) {
        if (val[i] <= '9' && val[i] >= '0') {
            num = (num * 10) + val[i] - '0';
        }
        else break;
    }
    return num;
}

ali_err_t ali_set_value_str(struct ali *self, const char* val) {
/* FINISH LATER */
}

void ali_print(const struct ali * self) {
    char buf [256]; /* :( */
}

/* Arithmetic */

ali_err_t ali_add(struct ali * dest, const struct ali * num1, const struct ali * num2) {
    if (!dest || !num1 || !num1->_number || !num2 || !num2->_number)
        return eALI_ERR_UNEXPECTEDNULL;
    size_t max_size = (num1->_len < num2->_len) ? num2->_len : num1->_len;
    for(size_t i = 0; i < max_size; i++) {
        size_t num1_sect = 0;
        size_t num2_sect = 0;
        /* FINISH LATER */
    }
}
