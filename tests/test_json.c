#include "../src/json.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static cJSON *parse_json(const char *text) {
  cJSON *root = NULL;
  assert(json_parse(text, &root) == 0);
  assert(root != NULL);
  return root;
}

/* ---------- json_parse ---------- */

static void test_json_parse_valid(void) {
  cJSON *root = NULL;

  assert(json_parse("{\"name\":\"Alice\"}", &root) == 0);
  assert(root != NULL);

  json_destroy(root);
}

static void test_json_parse_invalid(void) {
  cJSON *root = NULL;

  assert(json_parse("{ invalid json }", &root) == -1);
  assert(root == NULL);
}

/* ---------- json_get_string ---------- */

static void test_json_get_string_success(void) {
  cJSON *root = parse_json("{\"name\":\"Alice\"}");

  const char *value = json_get_string(root, "name");

  assert(value != NULL);
  assert(strcmp(value, "Alice") == 0);

  json_destroy(root);
}

static void test_json_get_string_missing_key(void) {
  cJSON *root = parse_json("{\"name\":\"Alice\"}");

  assert(json_get_string(root, "age") == NULL);

  json_destroy(root);
}

static void test_json_get_string_wrong_type(void) {
  cJSON *root = parse_json("{\"name\":123}");

  assert(json_get_string(root, "name") == NULL);

  json_destroy(root);
}

/* ---------- json_get_number ---------- */

static void test_json_get_number_success(void) {
  cJSON *root = parse_json("{\"age\":25}");

  double value = 0;

  assert(json_get_number(root, "age", &value) == 0);
  assert(value == 25.0);

  json_destroy(root);
}

static void test_json_get_number_missing_key(void) {
  cJSON *root = parse_json("{\"age\":25}");

  double value = 42.0;

  assert(json_get_number(root, "salary", &value) == -1);

  /* verify output parameter wasn't modified */
  assert(value == 42.0);

  json_destroy(root);
}

static void test_json_get_number_wrong_type(void) {
  cJSON *root = parse_json("{\"age\":\"25\"}");

  double value = 42.0;

  assert(json_get_number(root, "age", &value) == -1);
  assert(value == 42.0);

  json_destroy(root);
}

/* ---------- json_get_bool ---------- */

static void test_json_get_bool_true(void) {
  cJSON *root = parse_json("{\"stream\":true}");

  cJSON_bool value = cJSON_False;

  assert(json_get_bool(root, "stream", &value) == 0);
  assert(value);

  json_destroy(root);
}

static void test_json_get_bool_false(void) {
  cJSON *root = parse_json("{\"stream\":false}");

  cJSON_bool value = cJSON_True;

  assert(json_get_bool(root, "stream", &value) == 0);
  assert(!value);

  json_destroy(root);
}

static void test_json_get_bool_missing_key(void) {
  cJSON *root = parse_json("{\"stream\":true}");

  cJSON_bool value = cJSON_True;

  assert(json_get_bool(root, "missing", &value) == -1);

  /* verify output parameter wasn't modified */
  assert(value);

  json_destroy(root);
}

static void test_json_get_bool_wrong_type(void) {
  cJSON *root = parse_json("{\"stream\":\"true\"}");

  cJSON_bool value = cJSON_True;

  assert(json_get_bool(root, "stream", &value) == -1);
  assert(value);

  json_destroy(root);
}

/* ---------- object creation ---------- */

static void test_json_create_object(void) {
  cJSON *obj = json_create_object();

  assert(obj != NULL);
  assert(cJSON_IsObject(obj));

  json_destroy(obj);
}

static void test_json_create_array(void) {
  cJSON *array = json_create_array();

  assert(array != NULL);
  assert(cJSON_IsArray(array));

  json_destroy(array);
}

/* ---------- serialization ---------- */

static void test_json_print(void) {
  cJSON *obj = json_create_object();

  cJSON_AddStringToObject(obj, "name", "Alice");
  cJSON_AddNumberToObject(obj, "age", 25);

  char *text = json_print(obj);

  assert(text != NULL);

  /*
   * Don't compare the whole JSON string.
   * Formatting/order may change.
   */

  assert(strstr(text, "\"name\"") != NULL);
  assert(strstr(text, "\"Alice\"") != NULL);
  assert(strstr(text, "\"age\"") != NULL);

  json_free_string(text);
  json_destroy(obj);
}

/* ---------- null parameter tests ---------- */

static void test_json_get_string_null_object(void) {
  assert(json_get_string(NULL, "name") == NULL);
}

static void test_json_get_string_null_key(void) {
  cJSON *root = parse_json("{\"name\":\"Alice\"}");

  assert(json_get_string(root, NULL) == NULL);

  json_destroy(root);
}

static void test_json_get_number_null_output(void) {
  cJSON *root = parse_json("{\"age\":25}");

  assert(json_get_number(root, "age", NULL) == -1);

  json_destroy(root);
}

static void test_json_get_bool_null_output(void) {
  cJSON *root = parse_json("{\"stream\":true}");

  assert(json_get_bool(root, "stream", NULL) == -1);

  json_destroy(root);
}

int main(void) {
  test_json_parse_valid();
  test_json_parse_invalid();

  test_json_get_string_success();
  test_json_get_string_missing_key();
  test_json_get_string_wrong_type();

  test_json_get_number_success();
  test_json_get_number_missing_key();
  test_json_get_number_wrong_type();

  test_json_get_bool_true();
  test_json_get_bool_false();
  test_json_get_bool_missing_key();
  test_json_get_bool_wrong_type();

  test_json_create_object();
  test_json_create_array();

  test_json_print();

  test_json_get_string_null_object();
  test_json_get_string_null_key();
  test_json_get_number_null_output();
  test_json_get_bool_null_output();

  puts("All JSON tests passed.");

  return 0;
}