#include "memcpy_s.h"

memcpy_s_err_t memcpy_s(void* dest, size_t destsz, const void* src, size_t count) {
    memcpy_s_err_t error = MEMCPY_S_OK;

    // Check if either destination or source pointers are NULL
    if (dest == NULL || src == NULL) {
        error = MEMCPY_S_NULL_POINTER;
    }
    // Check if count or destination size is zero
    else if (count == 0 || destsz == 0) {
        error = MEMCPY_S_SIZE_OVERFLOW;
    }
    // Check if count is greater than destination size
    else if (count > destsz) {
        error = MEMCPY_S_SIZE_OVERFLOW;
    }
    // Check for overlap between source and destination
    else if ((dest < src && (char*)dest + count > (char*)src) ||
             (src < dest && (char*)src + count > (char*)dest)) {
        error = MEMCPY_S_OVERLAP_DETECTED;
    }
    else {
        // Perform the memory copy
        memcpy(dest, src, count);
    }

    return error;
}