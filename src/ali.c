#include "ali.h"
#include <stdlib.h>

void ali_init(struct ali * self) {
    self->_number = NULL;
    self->_len = 0;
}

void ali_deinit(struct ali * self) {
    free(self->_number);
}
