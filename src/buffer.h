#ifndef LLM_BUFFER_H
#define LLM_BUFFER_H
#include <stddef.h>

typedef struct {
  char *data;
  size_t length;
  size_t capacity;
} Buffer;

void buffer_init(Buffer *buf);
int buffer_append(Buffer *buf, const char *data, size_t length);
void buffer_clear(Buffer *buf);
void buffer_destroy(Buffer *buf);

#endif /* LLM_BUFFER_H */