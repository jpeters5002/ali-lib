#include <ali.h>
#include <stdio.h>

#define ENSURE(bool_expr, fail_formatstr, ...) \
    if (!(bool_expr)) { \
        printf("TESTS FAILED: " fail_formatstr "\n", ##__VA_ARGS__); \
        return 1; \
    } do {} while(0)

int main() {
    enum ali_err_t result;

    // add
    struct ali add_foos[3];
    for (size_t i = 0; i < 3; i++) {
        ali_init(&add_foos[i]);
    }
    ali_set_value_u64(&add_foos[1], SIZE_MAX);
    ali_set_value_u64(&add_foos[2], 1);
    result = ali_add(&add_foos[0], &add_foos[1], &add_foos[2]);
    ENSURE(result == eALI_ERR_NOERR,
        "ali_add returned error code: %s", str_from_ali_err(result));
    ENSURE(add_foos[0]._len == 2,
        "after adding 1 and SIZE_MAX (should overflow by 1), resulting length was not 2 (resulting length: %lu)", add_foos[0]._len);
    ENSURE(add_foos[0]._number[0] == 1 &&
        add_foos[0]._number[1] == 0,
        "result of adding 1 and SIZE_MAX is not the expected value; index 0: %lu; index 1: %lu, %lu", add_foos[0]._number[0], add_foos[0]._number[1], SIZE_MAX);
    for (size_t i = 0; i < 3; i++) {
        ali_deinit(&add_foos[i]);
    }

    // mult
    struct ali mult_foos[3];
    for (size_t i = 0; i < 3; i++) {
        ali_init(&mult_foos[i]);
    }
    ali_set_value_u64(&mult_foos[1], 9001);
    ali_set_value_u64(&mult_foos[2], 42);
    for (size_t mult_runs = 0; mult_runs < 15; mult_runs++) {
        result = ali_mult(&mult_foos[(0 + mult_runs) % 3], &mult_foos[(1 + mult_runs) % 3], &mult_foos[(2 + mult_runs) % 3]);
        ENSURE(result == eALI_ERR_NOERR, "ali_mult returned error code: %s", str_from_ali_err(result));
    }
    for (size_t i = 0; i < 3; i++) {
        ali_deinit(&mult_foos[i]);
    }

    // end
    printf("TESTS SUCCEEDED\n");

    return 0;
}
