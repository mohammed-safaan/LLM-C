#include "../src/buffer.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_buffer_init(void) {
  Buffer buf;

  buffer_init(&buf);

  assert(buf.data == NULL);
  assert(buf.length == 0);
  assert(buf.capacity == 0);
}

static void test_buffer_clear(void) {
  Buffer buf;

  buffer_init(&buf);

  buffer_append(&buf, "Hello", 5);

  buffer_clear(&buf);

  assert(buf.length == 0);
  assert(buf.capacity == 256); // memory is kept
  assert(strcmp(buf.data, "") == 0);

  buffer_destroy(&buf);
}

static void test_buffer_destroy(void) {
  Buffer buf;

  buffer_init(&buf);

  buffer_append(&buf, "Hello", 5);

  buffer_destroy(&buf);

  assert(buf.data == NULL);
  assert(buf.length == 0);
  assert(buf.capacity == 0);

  /* Destroy should be safe to call twice */
  buffer_destroy(&buf);

  assert(buf.data == NULL);
  assert(buf.length == 0);
  assert(buf.capacity == 0);
}

static void test_buffer_append_first(void) {
  Buffer buf;

  buffer_init(&buf);

  assert(buffer_append(&buf, "Hello", 5) == 0);

  assert(buf.capacity == 256);
  assert(buf.length == 5);
  assert(strcmp(buf.data, "Hello") == 0);

  buffer_destroy(&buf);
}

static void test_buffer_append_multiple(void) {
  Buffer buf;

  buffer_init(&buf);

  buffer_append(&buf, "Hello", 5);
  buffer_append(&buf, " ", 1);
  buffer_append(&buf, "World", 5);

  assert(buf.length == 11);
  assert(strcmp(buf.data, "Hello World") == 0);
  assert(buf.capacity == 256);

  buffer_destroy(&buf);
}

static void test_buffer_append_resize(void) {
  Buffer buf;

  buffer_init(&buf);

  char big[300];
  memset(big, 'A', sizeof(big));

  assert(buffer_append(&buf, big, sizeof(big)) == 0);

  assert(buf.length == 300);
  assert(buf.capacity == 512);
  assert(buf.data[300] == '\0');

  /* Verify every byte was copied correctly */
  for (size_t i = 0; i < sizeof(big); i++) {
    assert(buf.data[i] == 'A');
  }

  buffer_destroy(&buf);
}

static void test_buffer_append_large(void) {
  Buffer buf;

  buffer_init(&buf);

  char huge[5000];
  memset(huge, 'B', sizeof(huge));

  assert(buffer_append(&buf, huge, sizeof(huge)) == 0);

  assert(buf.length == 5000);
  assert(buf.capacity == 8192);
  assert(buf.data[5000] == '\0');

  /* Verify first and last bytes */
  assert(buf.data[0] == 'B');
  assert(buf.data[4999] == 'B');

  buffer_destroy(&buf);
}

static void test_buffer_append_empty(void) {
  Buffer buf;

  buffer_init(&buf);

  size_t old_capacity = buf.capacity;

  assert(buffer_append(&buf, "", 0) == 0);

  assert(buf.length == 0);
  assert(buf.capacity == old_capacity);
  assert(buf.data == NULL);

  buffer_destroy(&buf);
}

static void test_buffer_data_integrity_after_resize(void) {
  Buffer buf;

  buffer_init(&buf);

  buffer_append(&buf, "Hello", 5);

  char big[300];
  memset(big, 'X', sizeof(big));

  buffer_append(&buf, big, sizeof(big));

  /* Original data must still exist */
  assert(strncmp(buf.data, "Hello", 5) == 0);

  /* New data must immediately follow */
  for (size_t i = 0; i < sizeof(big); i++) {
    assert(buf.data[5 + i] == 'X');
  }

  buffer_destroy(&buf);
}

int main(void) {
  test_buffer_init();
  test_buffer_clear();
  test_buffer_destroy();

  test_buffer_append_first();
  test_buffer_append_multiple();
  test_buffer_append_resize();
  test_buffer_append_large();
  test_buffer_append_empty();
  test_buffer_data_integrity_after_resize();

  printf("All buffer tests passed.\n");
  return 0;
}