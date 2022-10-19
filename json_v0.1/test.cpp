#include <iostream>
#include <cstdlib>
#include <string>
#include "myjson.h"

static int main_ret = 0;
static int test_count = 0;
static int test_pass = 0;

#define EXPECT_EQ_BASE(equalify, expect, actual, format) \
    do{\
        test_count++;\
        if(equalify)\
            test_pass++;\
        else{\
            fprintf(stderr, "%s, %d: except:" format " actual: " format "\n", __FILE__, __LINE__, expect, actual);\
            main_ret = 1;\
        }\
    } while(0)

#define EXPECT_EQ_INT(expect, actual)  EXPECT_EQ_BASE((expect) == (actual), expect, actual, "%d")

static void test_parse_null(){
    json_value v;
    v.type = JSON_FALSE;
    EXPECT_EQ_INT(EXPECT::JSON_PARSE_OK, json_parse(&v,"null"));
}

static void test_parse_expect_value(){
    json_value v;
    v.type = JSON_FALSE;
    EXPECT_EQ_INT(EXPECT::JSON_PARSE_EXPECT_VALUE, json_parse(&v, ""));
    EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&v));

    v.type = JSON_FALSE;
    EXPECT_EQ_INT(EXPECT::JSON_PARSE_EXPECT_VALUE, json_parse(&v, " "));
    EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&v));
}

static void test_parse_invalid_value(){
    json_value v;
    v.type = JSON_FALSE;
    EXPECT_EQ_INT(EXPECT::JSON_PARSE_INVALID_VALUE, json_parse(&v, "nul"));
    EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&v));

    v.type = JSON_FALSE;
    EXPECT_EQ_INT(EXPECT::JSON_PARSE_INVALID_VALUE, json_parse(&v, "?"));
    EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&v));
}

static void test_parse_root_not_singular(){
    json_value v;
    v.type = JSON_FALSE;
    // 在直接测试中，我们希望返回的是不只有一个根节点，但是解析结果是ok（状态值0）
    EXPECT_EQ_INT(EXPECT::JSON_PARSE_ROOT_NOT_SINGULAR, json_parse(&v, "null x"));
    EXPECT_EQ_INT(json_type::JSON_NULL, json_get_type(&v));
}

static void test_parse(){
    test_parse_null();
    test_parse_expect_value();
    test_parse_invalid_value();
    test_parse_root_not_singular();
}

int main(){
    test_parse();
    printf("test\n");
    printf("%d/%d (%3.2f%%) passed\n", test_pass, test_count, test_pass * 100.0 / test_count);
    system("");
    return main_ret;
}