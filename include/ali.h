#ifndef ALI_H
#define ALI_H

#include <stddef.h>

struct ali {
    size_t * _number;
    size_t _len;
};

void init_ali(struct ali * self);

void deinit_ali(struct ali * self);

#endif // ALI_H
