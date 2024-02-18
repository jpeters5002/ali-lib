#include "ali_err.h"

const char *str_from_ali_err(ali_err_t value) {
    switch (value) {
        case eALI_ERR_NOERR:
            return "[NO ERROR]";
        case eALI_ERR_ALLOCFAIL:
            return "allocation fail";
        case eALI_ERR_UNEXPECTEDNULL:
            return "unexpected NULL pointer";
        case eALI_ERR_DIVBYZERO:
            return "divide by zero";
        case eALI_ERR_UNKNOWNERR:
            return "[UNKNOWN ERROR]";
    }
    return "[!] UNKNOWN ERROR CODE [!]";
}

