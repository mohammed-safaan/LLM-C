#include "buffer.h"
#include <stdlib.h>

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
