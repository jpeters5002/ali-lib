#ifndef ALI_H
#define ALI_H

#include <stddef.h>
#include <stdint.h>

struct ali {
    size_t * _number;
    size_t _len;
};

void ali_init(struct ali *self);
void ali_deinit(struct ali *self);
void ali_set_value_u64(struct ali *self, uint64_t val);

#endif // ALI_H
