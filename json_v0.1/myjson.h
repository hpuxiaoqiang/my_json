#ifndef MYJSON__H__
#define MYJSON__H__
#include <string>

//json数据类型
typedef enum {
    JSON_NULL, JSON_FALSE, JSON_TRUE, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT
} json_type;

//解析结果的类型
typedef enum {
    JSON_PARSE_OK = 0,
    JSON_PARSE_EXPECT_VALUE,
    JSON_PARSE_INVALID_VALUE,
    JSON_PARSE_ROOT_NOT_SINGULAR
}EXPECT;

//json的数据结构
typedef struct {
    json_type type;
}json_value;

int json_parse(json_value *v, const char* json);
json_type json_get_type(const json_value *v);


#endif