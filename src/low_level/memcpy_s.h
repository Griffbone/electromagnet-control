#ifndef MEMCPY_S_H
#define MEMCPY_S_H

#include "type_definitions.h"
#include <string.h>

typedef enum
{
  MEMCPY_S_OK = 0,
  MEMCPY_S_NULL_POINTER,
  MEMCPY_S_SIZE_OVERFLOW,
  MEMCPY_S_OVERLAP_DETECTED
} memcpy_s_err_t;

/**
 * @brief Copies memory from source to destination with safety checks.
 *
 * @param dest Pointer to the destination memory.
 * @param destsz Size of the destination memory.
 * @param src Pointer to the source memory.
 * @param count Number of bytes to copy.
 * @return memcpy_s_err_t Error code indicating the result of the operation.
 *
 * This function performs a memory copy from the source to the destination
 * with several safety checks to prevent common errors such as null pointers,
 * size overflows, and memory overlaps.
 */
memcpy_s_err_t memcpy_s(void *dest, size_t destsz, const void *src, size_t count);

void fill_array(void *array, const void *value, size_t count, const size_t element_size) {
  char *ptr = (char *)array; // Use byte-wise pointer arithmetic
  for (size_t i = 0; i < count; i++) {
    memcpy(ptr + i * element_size, value, element_size);
  }
}

#endif // MEMCPY_S_H