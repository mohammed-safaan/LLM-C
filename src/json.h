#include "../third_party/cJSON/cJSON.h"

const char *json_get_string(cJSON *obj, const char *key);
int json_get_number(cJSON *obj, const char *key, double *value);
int json_get_bool(cJSON *obj, const char *key, cJSON_bool *value);

int json_parse(const char *text, cJSON **root);
void json_destroy(cJSON *root);

cJSON *json_create_object(void);
cJSON *json_create_array(void);

char *json_print(cJSON *root);
void json_free_string(char *text);
