#ifndef ALI_ERR_H
#define ALI_ERR_H

typedef enum ali_err_t {
    eALI_ERR_NOERR = 0,
    eALI_ERR_ALLOCFAIL = -1000,
    eALI_ERR_UNEXPECTEDNULL,
    eALI_ERR_DIVBYZERO,
    eALI_ERR_UNKNOWNERR,
} ali_err_t;

const char *str_from_ali_err(ali_err_t value);

#endif // ALI_ERR_H

