#include "../src/buffer.h"
#include <assert.h>

int main(void) {
  Buffer buf;

  /* Test initialization */
  buffer_init(&buf);

  assert(buf.data == NULL);
  assert(buf.length == 0);
  assert(buf.capacity == 0);

  /* Test clear on empty buffer */
  buffer_clear(&buf);

  assert(buf.data == NULL);
  assert(buf.length == 0);
  assert(buf.capacity == 0);

  /* Test destroy on empty buffer */
  buffer_destroy(&buf);

  assert(buf.data == NULL);
  assert(buf.length == 0);
  assert(buf.capacity == 0);

  /* Test double destroy (should be safe) */
  buffer_destroy(&buf);

  assert(buf.data == NULL);
  assert(buf.length == 0);
  assert(buf.capacity == 0);

  return 0;
}