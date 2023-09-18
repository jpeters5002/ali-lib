#include "ali.h"
#include <stdlib.h>

void init_ali(struct ali * self) {
    self->_number = NULL;
    self->_len = 0;
}

void deinit_ali(struct ali * self) {
    free(self->_number);
}
