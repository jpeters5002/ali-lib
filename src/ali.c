#include "ali.h"
#include "ali_err.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

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
    size_t max_len = (num1->_len < num2->_len) ? num2->_len : num1->_len;
    const struct ali *nums[2] = {num1, num2};
    struct ali dest_proxy;
    struct ali *dest_proxy_ptr = dest;
    for (size_t num_idx = 0; num_idx < 2; num_idx++) {
        if (dest == nums[num_idx]) {
            dest_proxy_ptr = &dest_proxy;
            ali_init(dest_proxy_ptr);
            break;
        }
    }
    // now 'dest_proxy_ptr' points to either the argument 'dest' or a local initialized ali
    // and we test if 'dest_proxy_ptr' is not 'dest' to know if we need to copy 'dest_proxy' into 'dest'
    free(dest_proxy_ptr->_number);
    dest_proxy_ptr->_number = malloc(sizeof(size_t) * max_len);
    if (!dest_proxy_ptr->_number) {
        return eALI_ERR_ALLOCFAIL;
    }
    dest_proxy_ptr->_len = max_len;
    size_t overflow = 0;
    for(size_t i = 0; i < max_len; i++) {
        size_t num_sections[2] = {0, 0};
        for (size_t num_idx = 0; num_idx < 2; num_idx++) {
            if (i < nums[num_idx]->_len) {
                num_sections[num_idx] = nums[num_idx]->_number[nums[num_idx]->_len - i - 1];
            }
        }
        size_t *result_section = &(dest_proxy_ptr->_number[dest_proxy_ptr->_len - i - 1]);
        if (num_sections[0] > SIZE_MAX - num_sections[1]) {
            // overflow will occur (both numbers > 0 is guaranteed)
            *result_section = (SIZE_MAX - num_sections[1]) + num_sections[0] - 1 + overflow;
            overflow = 1;
        } else {
            // no overflow
            *result_section = num_sections[0] + num_sections[1] + overflow;
            overflow = 0;
        }
    }
    // apply overflow
    if (overflow != 0) {
        size_t *tmp = realloc(dest_proxy_ptr->_number, sizeof(size_t) * (max_len + 1));
        if (!tmp) {
            ali_deinit(dest_proxy_ptr);
            return eALI_ERR_ALLOCFAIL;
        }
        dest_proxy_ptr->_number = tmp;
        dest_proxy_ptr->_len = max_len + 1;
        memmove(dest_proxy_ptr->_number + 1, dest_proxy_ptr->_number, (dest_proxy_ptr->_len - 1) * sizeof(size_t));
        dest_proxy_ptr->_number[0] = overflow;
    }
    // apply proxy
    if (dest_proxy_ptr != dest) {
        free(dest->_number);
        dest->_number = malloc(sizeof(size_t) * dest_proxy_ptr->_len);
        if (!dest->_number) {
            return eALI_ERR_ALLOCFAIL;
        }
        dest->_len = dest_proxy_ptr->_len;
        memcpy(dest->_number, dest_proxy_ptr->_number, dest->_len * sizeof(size_t));
        ali_deinit(dest_proxy_ptr);
    }
    return eALI_ERR_NOERR;
}
