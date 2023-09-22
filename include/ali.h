#ifndef ALI_H
#define ALI_H

#include <stddef.h>

struct ali {
    size_t * _number;
    size_t _len;
};

void ali_init(struct ali * self);

void ali_deinit(struct ali * self);

#endif // ALI_H
