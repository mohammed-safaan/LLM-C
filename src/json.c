#include "json.h"

int json_parse(const char *text, cJSON **root) {
  if (text == NULL || root == NULL) {
    return -1;
  }
  *root = cJSON_Parse(text);
  if (*root == NULL) {
    return -1;
  }
  return 0;
};

void json_destroy(cJSON *root) { cJSON_Delete(root); }

const char *json_get_string(cJSON *obj, const char *key) {
  if (obj == NULL || key == NULL) {
    return NULL;
  }
  cJSON *item = cJSON_GetObjectItemCaseSensitive(obj, key);
  if (item == NULL || !cJSON_IsString(item)) {
    return NULL;
  }
  return cJSON_GetStringValue(item);
}

int json_get_number(cJSON *obj, const char *key, double *value) {
  if (obj == NULL || key == NULL || value == NULL) {
    return -1;
  }
  cJSON *item = cJSON_GetObjectItemCaseSensitive(obj, key);
  if (item == NULL || !cJSON_IsNumber(item)) {
    return -1;
  }
  *value = cJSON_GetNumberValue(item);
  return 0;
};

int json_get_bool(cJSON *obj, const char *key, cJSON_bool *value) {
  if (obj == NULL || key == NULL || value == NULL) {
    return -1;
  }
  cJSON *item = cJSON_GetObjectItemCaseSensitive(obj, key);
  if (item == NULL || !cJSON_IsBool(item)) {
    return -1;
  }
  *value = cJSON_IsTrue(item);
  return 0;
};

cJSON *json_create_object(void) { return cJSON_CreateObject(); };
cJSON *json_create_array(void) { return cJSON_CreateArray(); };
char *json_print(cJSON *root) { return cJSON_Print(root); };
void json_free_string(char *text) { cJSON_free(text); };