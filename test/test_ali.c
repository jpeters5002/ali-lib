#include <ali.h>
#include <stdio.h>

int main() {
    struct ali foo;
    ali_init(&foo);

    ali_set_value_u64(&foo, 0x123456789abcdef0ULL);
    printf("ali struct: len - %d, numptr - %p, num - %p\n", foo._len, foo._number, *foo._number);

    return 0;
}
