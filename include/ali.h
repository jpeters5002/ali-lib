#ifndef ALI_H
#define ALI_H

#include <stddef.h>
#include <stdint.h>

#include <ali_err.h>

struct ali {
    size_t *_number;
    size_t _len;
};

#define ALI_INIT_LIST() (struct ali) { NULL, 0 }
void ali_init(struct ali *self);
void ali_deinit(struct ali *self);
ali_err_t ali_set_value_u64(struct ali *self, uint64_t val);
ali_err_t ali_set_value_u32(struct ali *self, uint32_t val);
ali_err_t ali_set_value_u16(struct ali *self, uint16_t val);
ali_err_t ali_set_value_u8(struct ali *self, uint8_t val);

ali_err_t ali_set_value_str(struct ali *self, const char* val);
void ali_print(const struct ali * self);

/* Arithmetic */

ali_err_t ali_add(struct ali * dest, const struct ali * num1, const struct ali * num2);

#endif // ALI_H
