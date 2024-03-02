#include "ali.h"
#include "ali_err.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// stupid macro crap
#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#define RET_IF_ERR(result) \
    if (result != eALI_ERR_NOERR) return result

// ============================
// private functions
// ============================
bool _does_overflow_on_add(size_t l, size_t r) {
    return l > SIZE_MAX - r;
}

struct oa_t { size_t result; size_t overflow; };
struct oa_t _overflow_add(size_t l, size_t r) {
    struct oa_t oa;
    if (_does_overflow_on_add(l, r)) {
        oa.result = (SIZE_MAX - l) + r - 1;
        oa.overflow = 1;
    } else {
        oa.result = l + r;
        oa.overflow = 0;
    }
    return oa;
}

ali_err_t _ali_add_into_index(struct ali *dest, size_t addend, size_t index) {
    assert(dest->_len > index);
    struct oa_t oa = {0, 0};
    size_t overflow_first_addition;
    for (size_t i = index; i < dest->_len; i++) {
        size_t *section = &(dest->_number[dest->_len - 1 - i]);
        oa = _overflow_add(addend, oa.overflow);
        overflow_first_addition = oa.overflow;
        oa = _overflow_add(*section, oa.result);
        *section = oa.result;
        if (overflow_first_addition || oa.overflow) {
            oa.overflow = 1;
            addend = 0;
        } else {
            return eALI_ERR_NOERR;
        }
    }
    assert(!"Unreachable statement -- file ali.c; line " STRINGIFY(__LINE__));
    return eALI_ERR_UNKNOWNERR;
}

ali_err_t _apply_proxy(struct ali *proxy, struct ali *dest) {
    if (proxy != dest) {
        free(dest->_number);
        dest->_number = malloc(sizeof(size_t) * proxy->_len);
        if (!dest->_number) {
            return eALI_ERR_ALLOCFAIL;
        }
        dest->_len = proxy->_len;
        memcpy(dest->_number, proxy->_number, dest->_len * sizeof(size_t));
        ali_deinit(proxy);
    }
    return eALI_ERR_NOERR;
}

size_t _ali_absdiff_expect_size(const struct ali * num1, const struct ali * num2)
{
    // TODO: implement
}

// ============================
// public functions
// ============================
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
//    DEFINE_PRIMITIVE_ALI_SET_VALUE(uint64_t)
    if (self->_number == NULL) {
        self->_number = malloc(sizeof(size_t) * RATIO_PRIMITIVE_TO_SIZE_T_RU(uint64_t));
        if (self->_number == NULL) {
            /* malloc fail */
            return eALI_ERR_ALLOCFAIL;
        }
    } else if (self->_len != RATIO_PRIMITIVE_TO_SIZE_T_RU(uint64_t)) {
        void *tmp = realloc(self->_number, sizeof(size_t) * RATIO_PRIMITIVE_TO_SIZE_T_RU(uint64_t));
        if (tmp == NULL) {
            /* realloc fail */
            return eALI_ERR_ALLOCFAIL;
        }
        self->_number = tmp;
    }
    self->_len = RATIO_PRIMITIVE_TO_SIZE_T_RU(uint64_t);
    for (size_t i = 0; i < RATIO_PRIMITIVE_TO_SIZE_T_RU(uint64_t); i++) {
        /* reverse index access; set to the value shifted properly and casted (potentially down) to a size_t */
        self->_number[RATIO_PRIMITIVE_TO_SIZE_T_RU(uint64_t) - 1 - i] = (size_t) (val >> (8 * i * sizeof(size_t)));
    }
    return eALI_ERR_NOERR;
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

// ============================
// Arithmetic
// ============================

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
        // TODO: this check doesn't account for the 'overflow' variable
        if (num_sections[0] > SIZE_MAX - num_sections[1]) {
            // overflow will occur (both numbers > 0 is guaranteed)
            *result_section = (SIZE_MAX - num_sections[0]) + num_sections[1] - 1 + overflow;
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

ali_err_t ali_add_size(struct ali * dest, const struct ali * num1, size_t num2)
{
    // TODO: implement
}

ali_err_t ali_mult (struct ali * dest, const struct ali * num1, const struct ali * num2) {
    if (!dest || !num1 || !num1->_number || !num2 || !num2->_number)
        return eALI_ERR_UNEXPECTEDNULL;
    size_t max_dest_len = num1->_len + num2->_len;
    const struct ali * nums[2] = {num1, num2};
    struct ali alternate;
    struct ali *proxy = dest;
    for (size_t num_idx = 0; num_idx < 2; num_idx++) {
        if (dest == nums[num_idx]) {
            proxy = &alternate;
            ali_init(proxy);
            break;
        }
    }
    // 'proxy' is either the argument 'dest' or points to the local initialized ali 'alternate'
    free(proxy->_number);
    proxy->_number = calloc(sizeof(size_t) * max_dest_len, 0);
    if (!proxy->_number)
        return eALI_ERR_ALLOCFAIL;
    proxy->_len = max_dest_len;
    //Half section index (weird)
    for (size_t hsi1 = 0; hsi1 < num1->_len * 2; hsi1++) {
        for (size_t hsi2 = 0; hsi2 < num2->_len * 2; hsi2++) {
            size_t whichhalf[2] = {hsi1 % 2, hsi2 % 2}; // 0 is right half, 1 is left half -- indexed by num_idx
            size_t fsi[2] = {hsi1 / 2, hsi2 / 2}; // full section index -- indexed by num_idx

            size_t hs_num[2]; // half section num -- indexed by num_idx
            for (size_t num_idx = 0; num_idx < 2; num_idx++) {
                hs_num[num_idx] = nums[num_idx]->_number[nums[num_idx]->_len - fsi[num_idx] - 1];
                if (whichhalf[num_idx] == 0) {
                    hs_num[num_idx] &=0xffffffff;
                } else {
                    hs_num[num_idx] >>= 4 * 8;
                }
            }
            size_t hsi_dest = hsi1 + hsi2;
            size_t result = hs_num[0] * hs_num[1];
            if (hsi_dest % 2 == 0) {
                //TODO: Make the following function
                _ali_add_into_index(proxy, result, hsi_dest / 2);
            } else {
                _ali_add_into_index(proxy, result& 0xffffffff, hsi_dest / 2);
                _ali_add_into_index(proxy, result >> 4 * 8, (hsi_dest / 2) + 1);
            }
        }
    }
    // clamp allocated space
    size_t zero_count = 0;
    for (size_t i = 0; i < proxy->_len; i++) {
        if (proxy->_number[i] == 0) {
            zero_count++;
        } else {
            break;
        }
    }
    if (zero_count > 0) {
        // TODO: make sure this doesn't cause problems with one of the ali's being 0
        // (maybe we can check each for zero earlier in the function)
        memmove(proxy->_number, proxy->_number + zero_count, (proxy->_len - zero_count) * sizeof(size_t));
        size_t new_len = proxy->_len - zero_count;
        if (new_len == 0) {
            void *tmp = realloc(proxy->_number, 1 * sizeof(size_t));
            if (!tmp) {
                goto zero_count_bad_alloc;
            }
            proxy->_number = tmp;
            proxy->_len = 1;
            proxy->_number[0] = 0;
        } else {
            void *tmp = realloc(proxy->_number, new_len * sizeof(size_t));
            if (!tmp) {
                goto zero_count_bad_alloc;
            }
            proxy->_number = tmp;
            proxy->_len = new_len;
        }
        goto zero_count_no_bad_alloc;
        zero_count_bad_alloc:
        // TODO: I think there may need to be more cleanup here
        return eALI_ERR_ALLOCFAIL;
        zero_count_no_bad_alloc: do {} while (0);
    }

    ali_err_t subresult = _apply_proxy(proxy, dest);
    return subresult;
}

ali_err_t ali_div_size(struct ali * dest_div, size_t *dest_mod, const struct ali * numerator, const size_t denominator) {
    if ((!dest_div && !dest_mod) || !numerator || !numerator->_number) {
        return eALI_ERR_UNEXPECTEDNULL;
    }
    if (denominator == 0) {
        return eALI_ERR_DIVBYZERO;
    }
    ali_err_t subresult;
    if (numerator->_len == 1 && numerator->_number[0] == 0) {
        // numerator is zero
        free(dest_div->_number);
        dest_div->_number = calloc(1 * sizeof(size_t), 0);
        if (!dest_div->_number) {
            return eALI_ERR_ALLOCFAIL;
        }
        dest_div->_len = 1;
        return eALI_ERR_NOERR;
    }
    struct ali *proxy = dest_div;
    // TODO: do the rest of proxy setup
    free(proxy->_number);
    proxy->_number = calloc(sizeof(size_t) * numerator->_len, 0);
    if (!proxy->_number) {
        return eALI_ERR_ALLOCFAIL;
    }
    proxy->_len = numerator->_len;
    struct ali product, a_den, a_lb, a_pacs; // denominator, lower_bound, previous_and_current_section
    ali_init(&product);
    ali_init(&a_den);
    ali_init(&a_lb);
    ali_init(&a_pacs);
    subresult = ali_set_value_u64(&a_den, denominator);
    RET_IF_ERR(subresult); // TODO: we should deinit everything instead of just returning
    size_t section_div_remainder = 0;
    for (size_t numerator_idx = 0; numerator_idx < numerator->_len; numerator_idx++) {
        size_t *result_section = proxy->_number + numerator_idx;
        const size_t numerator_section_number = numerator->_number[numerator_idx];
        if (section_div_remainder == 0) {
            *result_section = numerator_section_number / denominator;
            section_div_remainder = numerator_section_number % denominator;
        } else {
            a_pacs._number = malloc(sizeof(size_t) * 2);
            if (!a_pacs._number) {
                return eALI_ERR_ALLOCFAIL;
            }
            a_pacs._len = 2;
            a_pacs._number[0] = section_div_remainder;
            a_pacs._number[1] = numerator_section_number;
            subresult = ali_set_value_u64(&a_lb, numerator_section_number / denominator);
            RET_IF_ERR(subresult);
            while (1) {
                // TODO: create function ali_add_size
                ali_add_size(&a_lb, &a_lb, 1);
                subresult = ali_mult(&product, &a_lb, &a_pacs);
                RET_IF_ERR(subresult);
                // check if product >= a_pacs
                int cmp;
                subresult = ali_cmp(&cmp, &a_pacs, &a_lb);
                RET_IF_ERR(subresult);
                switch (cmp) {
                    case -1: // TODO: make sure this means that a_pacs < a_lb
                        *result_section = a_lb._number[0] - 1;
                        section_div_remainder = _ali_absdiff_expect_size(&a_lb, &a_pacs); // TODO: write function
                        goto break_while_loop;
                    case 0:
                        *result_section = a_lb._number[0];
                        section_div_remainder = 0;
                        goto break_while_loop;
                    default:
                        break;
                }
            }
            break_while_loop: do {} while (0);
        }
    }
    if (dest_mod) {
        *dest_mod = section_div_remainder;
    }
    //_apply_proxy(); // help
    return eALI_ERR_NOERR;
}

// ============================
// Other Operations
// ============================

ali_err_t ali_cmp(int * result, const struct ali * l, const struct ali * r)
{
    // TODO: implement
}
