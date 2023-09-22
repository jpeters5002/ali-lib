#include "ali.h"
#include <stdlib.h>

#define RATIO_PRIMITIVE_TO_SIZE_T_RU(primitive) \
    ((sizeof(primitive) / sizeof(size_t)) ? (sizeof(primitive) / sizeof(size_t)) : 1)

void ali_init(struct ali *self) {
    self->_number = NULL;
    self->_len = 0;
}

void ali_deinit(struct ali *self) {
    free(self->_number);
}

#define RATIO_U64 RATIO_PRIMITIVE_TO_SIZE_T_RU(uint64_t)
void ali_set_value_u64(struct ali *self, uint64_t val) {
    if (self->_number == NULL) {
        self->_number = malloc(sizeof(size_t) * RATIO_U64);
        if (self->_number == NULL) {
            // TODO: this function errored due to a failed malloc call; this should eventually return a value that the calling function can check
            return;
        }
    } else {
        void *tmp = realloc(self->_number, sizeof(size_t) * RATIO_U64);
        if (tmp == NULL) {
            // realloc fail; try malloc
            free(self->_number);
            self->_number = malloc(sizeof(size_t) * RATIO_U64);
            if (self->_number == NULL) {
                // TODO: add error value for return
                return;
            }
        }
    }
    self->_len = RATIO_U64;
    for (size_t i = 0; i < RATIO_U64; i++) {
        // reverse index access; set to the value shifted properly and casted (potentially down) to a size_t
        self->_number[RATIO_U64 - 1 - i] = (size_t) (val >> (8 * i * RATIO_U64));
    }
}
