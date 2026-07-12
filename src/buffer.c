#include "buffer.h"
#include <stdlib.h>
#include <string.h>

void buffer_init(Buffer *buf) {
  buf->capacity = 0;
  buf->length = 0;
  buf->data = NULL;
}

void buffer_clear(Buffer *buf) {
  buf->length = 0;
  if (buf->data) {
    buf->data[0] = '\0';
  }
}

void buffer_destroy(Buffer *buf) {
  free(buf->data);
  buf->data = NULL;
  buf->length = 0;
  buf->capacity = 0;
}

int buffer_append(Buffer *buf, const char *data, size_t length) {
  if (length == 0) {
    return 0;
  }
  if (length > SIZE_MAX - buf->length - 1) {
    return -1;
  }

  size_t required = buf->length + length + 1;

  if (required > buf->capacity) {
    size_t new_capacity = (buf->capacity == 0) ? 256 : buf->capacity;

    while (new_capacity < required) {
      new_capacity *= 2;
    }

    char *new_data = realloc(buf->data, new_capacity);

    if (!new_data)
      return -1;

    buf->data = new_data;
    buf->capacity = new_capacity;
  }

  memcpy(buf->data + buf->length, data, length);

  buf->length += length;
  buf->data[buf->length] = '\0';

  return 0;
}