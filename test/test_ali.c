#include <ali.h>
#include <stdio.h>

#define ENSURE(bool_expr, fail_formatstr, ...) \
    if (!(bool_expr)) { \
        printf("TESTS FAILED: " fail_formatstr "\n", ##__VA_ARGS__); \
        return 1; \
    } do {} while(0)

int main() {
    // add
    struct ali add_foos[3];
    for (size_t i = 0; i < 3; i++) {
        ali_init(&add_foos[i]);
    }
    ali_set_value_u64(&add_foos[1], SIZE_MAX);
    ali_set_value_u64(&add_foos[2], 1);
    enum ali_err_t result = ali_add(&add_foos[0], &add_foos[1], &add_foos[2]);
    ENSURE(result == eALI_ERR_NOERR, "ali_add returned error code: %s", str_from_ali_err(result));
    ENSURE(add_foos[0]._len == 2, "after adding 1 and SIZE_MAX (should overflow by 1), resulting length was not 2 (resulting length: %lu)", add_foos[0]._len);
    ENSURE(add_foos[0]._number[0] == 1 && add_foos[1]._number[1] == 0, "result of adding 1 and SIZE_MAX is not the expected value");

    printf("TESTS SUCCEEDED\n");
    return 0;
}
